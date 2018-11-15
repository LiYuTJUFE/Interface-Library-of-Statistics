/* Program usage: mpiexec -n 1 rosenbrock1 [-help] [all TAO options] */

/*  Include "petsctao.h" so we can use TAO solvers.  */
#include <petsctao.h>

static  char help[] = "This example demonstrates use of the TAO package to \n\
solve an unconstrained minimization problem on a single processor.  We \n\
minimize the extended Rosenbrock function: \n\
   sum_{i=0}^{n/2-1} ( alpha*(x_{2i+1}-x_{2i}^2)^2 + (1-x_{2i})^2 ) \n\
or the chained Rosenbrock function:\n\
   sum_{i=0}^{n-1} alpha*(x_{i+1} - x_i^2)^2 + (1 - x_i)^2\n";

/*T
   Concepts: TAO^Solving an unconstrained minimization problem
   Routines: TaoCreate();
   Routines: TaoSetType(); TaoSetObjectiveAndGradientRoutine();
   Routines: TaoSetHessianRoutine();
   Routines: TaoSetInitialVector();
   Routines: TaoSetFromOptions();
   Routines: TaoSolve();
   Routines: TaoDestroy();
   Processors: 1
T*/




/*
   User-defined application context - contains data needed by the
   application-provided call-back routines that evaluate the function,
   gradient, and hessian.
*/
typedef struct {
  PetscInt  n;          /* dimension */
  PetscReal alpha;   /* condition parameter */
  PetscBool chained;
} AppCtx;

/* -------------- User-defined routines ---------- */
PetscErrorCode FormFunctionObjective(Tao,Vec,PetscReal*,void*);

int main(int argc,char **argv)
{
  PetscErrorCode     ierr;                  /* used to check for functions returning nonzeros */
  PetscReal          zero=0.0;
  Vec                x;                     /* solution vector */
  Mat                H;
  Tao                tao;                   /* Tao solver context */
  PetscBool          flg, test_lmvm = PETSC_FALSE;
  PetscMPIInt        size,rank;                  /* number of processes running */
  AppCtx             user;                  /* user-defined application context */
  KSP                ksp;
  PC                 pc;
  Mat                M;
  Vec                in, out, out2;
  PetscReal          mult_solve_dist;

  /* Initialize TAO and PETSc */
  ierr = PetscInitialize(&argc,&argv,(char*)0,help);if (ierr) return ierr;
  ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);CHKERRQ(ierr);
  ierr = MPI_Comm_rank(PETSC_COMM_WORLD,&rank);CHKERRQ(ierr);
  if (size >1) SETERRQ(PETSC_COMM_SELF,1,"Incorrect number of processors");

  /* Initialize problem parameters */
  user.n = 2; user.alpha = 99.0; user.chained = PETSC_FALSE;
  /* Check for command line arguments to override defaults */
  ierr = PetscOptionsGetInt(NULL,NULL,"-n",&user.n,&flg);CHKERRQ(ierr);
  ierr = PetscOptionsGetReal(NULL,NULL,"-alpha",&user.alpha,&flg);CHKERRQ(ierr);
  ierr = PetscOptionsGetBool(NULL,NULL,"-chained",&user.chained,&flg);CHKERRQ(ierr);
  ierr = PetscOptionsGetBool(NULL,NULL,"-test_lmvm",&test_lmvm,&flg);CHKERRQ(ierr);

  /* Allocate vectors for the solution and gradient */
  ierr = VecCreateSeq(PETSC_COMM_SELF,user.n,&x);CHKERRQ(ierr);
  ierr = MatCreateSeqAIJ(PETSC_COMM_SELF,user.n,user.n,1,NULL,&H);CHKERRQ(ierr);

  /* The TAO code begins here */

  /* Create TAO solver with desired solution method */
  ierr = TaoCreate(PETSC_COMM_SELF,&tao);CHKERRQ(ierr);
  ierr = TaoSetType(tao,TAOLMVM);CHKERRQ(ierr);

  /* Set solution vec and an initial guess */
  ierr = VecSet(x, zero);CHKERRQ(ierr);
  ierr = TaoSetInitialVector(tao,x);CHKERRQ(ierr);
  ierr = VecView(x,PETSC_VIEWER_STDOUT_SELF);CHKERRQ(ierr);

  /* Set routines for function, gradient, hessian evaluation */
  ierr = TaoSetObjectiveRoutine(tao,FormFunctionObjective,&user);CHKERRQ(ierr);
  ierr = TaoSetGradientRoutine(tao,TaoDefaultComputeGradient,NULL);CHKERRQ(ierr);
  ierr = TaoSetHessianRoutine(tao,H,H,TaoDefaultComputeHessian,NULL);CHKERRQ(ierr);
  
  /* Test the LMVM matrix */
  if (test_lmvm) {
    ierr = PetscOptionsSetValue(NULL, "-tao_type", "bqnktr");CHKERRQ(ierr);
  }

  /* Check for TAO command line options */
  ierr = TaoSetFromOptions(tao);CHKERRQ(ierr);

  /* SOLVE THE APPLICATION */
  ierr = TaoSolve(tao);CHKERRQ(ierr);
  ierr = VecView(x,PETSC_VIEWER_STDOUT_SELF);CHKERRQ(ierr);
  
  /* Test the LMVM matrix */
  if (test_lmvm) {
    ierr = TaoGetKSP(tao, &ksp);CHKERRQ(ierr);
    ierr = KSPGetPC(ksp, &pc);CHKERRQ(ierr);
    ierr = PCLMVMGetMatLMVM(pc, &M);CHKERRQ(ierr);
    ierr = VecDuplicate(x, &in);CHKERRQ(ierr);
    ierr = VecDuplicate(x, &out);CHKERRQ(ierr);
    ierr = VecDuplicate(x, &out2);CHKERRQ(ierr);
    ierr = VecSet(in, 1.0);CHKERRQ(ierr);
    ierr = MatMult(M, in, out);CHKERRQ(ierr);
    ierr = MatSolve(M, out, out2);CHKERRQ(ierr);
    ierr = VecAXPY(out2, -1.0, in);CHKERRQ(ierr);
    ierr = VecNorm(out2, NORM_2, &mult_solve_dist);CHKERRQ(ierr);
    if (mult_solve_dist < 1.e-11) {
      ierr = PetscPrintf(PetscObjectComm((PetscObject)tao), "error between LMVM MatMult and MatSolve: < 1.e-11\n");CHKERRQ(ierr);
    } else if(mult_solve_dist < 1.e-6) {
      ierr = PetscPrintf(PetscObjectComm((PetscObject)tao), "error between LMVM MatMult and MatSolve: < 1.e-6\n");CHKERRQ(ierr);
    } else {
      ierr = PetscPrintf(PetscObjectComm((PetscObject)tao), "error between LMVM MatMult and MatSolve: %e\n", (double)mult_solve_dist);CHKERRQ(ierr);
    }
    ierr = VecDestroy(&in);CHKERRQ(ierr);
    ierr = VecDestroy(&out);CHKERRQ(ierr);
    ierr = VecDestroy(&out2);CHKERRQ(ierr);
  }

  ierr = TaoDestroy(&tao);CHKERRQ(ierr);
  ierr = VecDestroy(&x);CHKERRQ(ierr);
  ierr = MatDestroy(&H);CHKERRQ(ierr);

  ierr = PetscFinalize();
  return ierr;
}

/* -------------------------------------------------------------------- */
/*
    FormFunctionGradient - Evaluates the function, f(X).

    Input Parameters:
.   tao  - the Tao context
.   X    - input vector
.   ptr  - optional user-defined context, as set by TaoSetFunctionGradient()

    Output Parameters:
.   f - function value

    Note:
    Some optimization methods ask for the function and the gradient evaluation
    at the same time.  Evaluating both at once may be more efficient that
    evaluating each separately.
*/
PetscErrorCode FormFunctionObjective(Tao tao,Vec X,PetscReal *f,void *ptr)
{
  AppCtx            *user = (AppCtx *) ptr;
  PetscInt          i,nn=user->n/2;
  PetscErrorCode    ierr;
  PetscReal         ff=0,t1,t2,alpha=user->alpha;
  const PetscScalar *x;

  PetscFunctionBeginUser;
  /* Get pointers to vector data */
  ierr = VecGetArrayRead(X,&x);CHKERRQ(ierr);

  /* Compute f(X) */
  if (user->chained) {
    for (i=0; i<user->n-1; i++) {
      t1 = x[i+1] - x[i]*x[i];
      ff += PetscSqr(1 - x[i]) + alpha*t1*t1;
    }
  } else {
    for (i=0; i<nn; i++){
      t1 = x[2*i+1]-x[2*i]*x[2*i]; t2= 1-x[2*i];
      ff += alpha*t1*t1 + t2*t2;
    }
  }

  /* Restore vectors */
  ierr = VecRestoreArrayRead(X,&x);CHKERRQ(ierr);
  *f   = ff;

  ierr = PetscLogFlops(nn*10);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}

/*TEST

   build:
      requires: !complex

   test:
      args: -tao_smonitor -tao_type nls -tao_gatol 1.e-4
      requires: !single

   test:
      suffix: 2
      args: -tao_smonitor -tao_type lmvm -tao_gatol 1.e-3

   test:
      suffix: 3
      args: -tao_smonitor -tao_type ntr -tao_gatol 1.e-4
      requires: !single

   test:
      suffix: 4
      args: -tao_smonitor -tao_type ntr -tao_mf_hessian -pc_type none -tao_gatol 1.e-4
      
   test:
      suffix: 5
      args: -tao_smonitor -tao_type bntr -tao_gatol 1.e-4
      
   test:
      suffix: 6
      args: -tao_smonitor -tao_type bntl -tao_gatol 1.e-4
   
   test:
      suffix: 7
      args: -tao_smonitor -tao_type bnls -tao_gatol 1.e-4
   
   test:
      suffix: 8
      args: -tao_smonitor -tao_type bntr -tao_bnk_max_cg_its 3 -tao_gatol 1.e-4
   
   test:
      suffix: 9
      args: -tao_smonitor -tao_type bntl -tao_bnk_max_cg_its 3 -tao_gatol 1.e-4
   
   test:
      suffix: 10
      args: -tao_smonitor -tao_type bnls -tao_bnk_max_cg_its 3 -tao_gatol 1.e-4
      
   test:
      suffix: 11
      args: -test_lmvm -tao_max_it 10 -tao_bqnk_mat_type lmvmbrdn
      
   test:
      suffix: 12
      args: -test_lmvm -tao_max_it 10 -tao_bqnk_mat_type lmvmbadbrdn
      
   test:
     suffix: 13
     args: -test_lmvm -tao_max_it 10 -tao_bqnk_mat_type lmvmsymbrdn

   test:
     suffix: 14
     args: -test_lmvm -tao_max_it 10 -tao_bqnk_mat_type lmvmbfgs
     
   test:
     suffix: 15
     args: -test_lmvm -tao_max_it 10 -tao_bqnk_mat_type lmvmdfp
     
   test:
     suffix: 16
     args: -test_lmvm -tao_max_it 10 -tao_bqnk_mat_type lmvmsr1
     
   test:
     suffix: 17
     args: -tao_smonitor -tao_gatol 1e-4 -tao_type bqnls
     
   test:
     suffix: 18
     args: -tao_smonitor -tao_gatol 1e-4 -tao_type blmvm
     
   test:
     suffix: 19
     args: -tao_smonitor -tao_gatol 1e-4 -tao_type bqnktr -tao_bqnk_mat_type lmvmsr1
     
   test:
     suffix: 20
     args: -tao_monitor -tao_gatol 1e-4 -tao_type blmvm -tao_ls_monitor
     
   test:
     suffix: 21
     args: -test_lmvm -tao_max_it 10 -tao_bqnk_mat_type lmvmsymbadbrdn

TEST*/
