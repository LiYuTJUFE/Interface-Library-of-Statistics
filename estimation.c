/*
 * =====================================================================================
 *
 *       Filename:  estimation.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2018 01:46:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <petsctao.h>

#include "model.h"

static  char help[] = "STAT\n";

typedef struct {
   MODEL_Data      real_data; 
   MODEL_Data      simu_data;
   MODEL_Data      resi_data;
   MODEL_Variables variables;
} AppCtx;

PetscErrorCode FormFunctionObjective(Tao tao, Vec X, PetscReal *f, void *ptr)
{
   AppCtx            *user = (AppCtx *) ptr;
   PetscErrorCode    ierr;
   const PetscScalar *x;
   PetscScalar       *para = MODEL_VariablesGetParameters(user->variables);
   /* Get pointers to vector data */
   ierr = VecGetArrayRead(X,&x);CHKERRQ(ierr);
   /* Update para and compute f(X) */
   for (int i = 0; i < MODEL_VariablesGetNumParameters(user->variables); ++i)
   {
      para[i] = x[i];
   }
   //MODEL_VariablesSetActiveParameters(user->variables, x, -1, NULL);
//   MODEL_VariablesPrint(user->variables);
   MODEL_Simulation(user->simu_data, user->variables, user->real_data, NULL, 0);
//   MODEL_DataPrint(user->simu_data);
//   MODEL_DataPrint(user->real_data);
   *f = MODEL_EvalResiduals(user->resi_data, user->real_data, user->simu_data, user->variables);
   /* Restore vectors */
   ierr = VecRestoreArrayRead(X,&x);CHKERRQ(ierr);
//   printf ( "res = %e\n", *f );

   return ierr;
}


PetscErrorCode MODEL_Estimation(MODEL_Data real_data, MODEL_Data simu_data, MODEL_Data resi_data, 
      MODEL_Variables variables, int argc,char **argv)
{
   AppCtx             user  = {real_data, simu_data, resi_data, variables};
   PetscInt           n     = MODEL_VariablesGetNumParameters(variables);
   PetscScalar        *para = MODEL_VariablesGetParameters(variables);
   /* --------------------------------------------------------------------------------------- */
   PetscInt           i;
   PetscErrorCode     ierr;                  /* used to check for functions returning nonzeros */
   Tao                tao;                   /* Tao solver context */
   Vec                X;                     /* solution vector */
   Mat                H;                     /* */
   PetscMPIInt        size,rank;             /* number of processes running */
   PetscScalar        *x;
   /* Initialize TAO and PETSc */
   ierr = PetscInitialize(&argc,&argv,(char*)0,help);if (ierr) return ierr;
   ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);CHKERRQ(ierr);
   ierr = MPI_Comm_rank(PETSC_COMM_WORLD,&rank);CHKERRQ(ierr);
   if (size >1) SETERRQ(PETSC_COMM_SELF,1,"Incorrect number of processors");
   /* Allocate vectors for the solution and gradient */
   ierr = VecCreateSeq(PETSC_COMM_SELF,n,&X);CHKERRQ(ierr);
   ierr = MatCreateSeqAIJ(PETSC_COMM_SELF,n,n,1,NULL,&H);CHKERRQ(ierr);
   /* Create TAO solver with desired solution method */
   ierr = TaoCreate(PETSC_COMM_SELF,&tao);CHKERRQ(ierr);
   /* Limited Memory Variable Metric method is a quasi-Newton optimization solver for unconstrained minimization */
   ierr = TaoSetType(tao,TAOLMVM);CHKERRQ(ierr);
   /* Set solution vec */
   VecGetArray(X, &x);
   for (i = 0; i < n; ++i)
   {
      x[i] = para[i];
   }
   VecRestoreArray(X, &x);
//   ierr = VecView(X,PETSC_VIEWER_STDOUT_SELF);CHKERRQ(ierr);
   /* Set an initial guess of TAO */
   ierr = TaoSetInitialVector(tao,X);CHKERRQ(ierr);
   /* Set routines for function, gradient, hessian evaluation */
   ierr = TaoSetObjectiveRoutine(tao,FormFunctionObjective,&user);CHKERRQ(ierr);
   ierr = TaoSetGradientRoutine(tao,TaoDefaultComputeGradient,NULL);CHKERRQ(ierr);
   ierr = TaoSetHessianRoutine(tao,H,H,TaoDefaultComputeHessian,NULL);CHKERRQ(ierr);
   /* Check for TAO command line options */
   ierr = TaoSetFromOptions(tao);CHKERRQ(ierr);
   /* SOLVE THE APPLICATION */
   ierr = TaoSolve(tao);CHKERRQ(ierr);
//   ierr = VecView(X,PETSC_VIEWER_STDOUT_SELF);CHKERRQ(ierr);
   /* Destroy X, H and Tao */
   ierr = TaoDestroy(&tao);CHKERRQ(ierr);
   ierr = VecDestroy(&X);CHKERRQ(ierr);
   ierr = MatDestroy(&H);CHKERRQ(ierr);
   ierr = PetscFinalize();
   /* --------------------------------------------------------------------------------------- */
   return ierr;
}
/*
PetscErrorCode FromParaToVec(PetscReal *para, Vec X)
{
   const PetscScalar *x;
   ierr = VecRestoreArrayRead(X,&x);CHKERRQ(ierr);
}
PetscErrorCode FromVecToPara(Vec X, PetscReal *para)
{
   const PetscScalar *x;
   ierr = VecGetArrayRead(X,&x);CHKERRQ(ierr);
}
*/

double MODEL_EvalResiduals(MODEL_Data resi_data, MODEL_Data real_data, MODEL_Data simu_data, MODEL_Variables variables)
{
   double  resi   = 0;
   double *resi_Y = MODEL_DataGetY(resi_data, 0);
   double *real_Y = MODEL_DataGetY(real_data, 0);
   double *simu_Y = MODEL_DataGetY(simu_data, 0);
   int     size   = MODEL_DataGetT(resi_data)*MODEL_DataGetDim(resi_data);
   /* resi_Y = real_Y - simu_Y */
   copy(real_Y, resi_Y, size);
   axpy(-1.0, simu_Y, resi_Y, size);
   resi = dot(resi_Y, resi_Y, size);
   return resi;
}
