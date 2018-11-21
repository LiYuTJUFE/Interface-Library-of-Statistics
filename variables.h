/*
 * =====================================================================================
 *
 *       Filename:  variables.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2018 07:35:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  _VARIABLES_H_
#define  _VARIABLES_H_

#include "utilities.h"

typedef struct model_Variables_struct
{
   char    name[64];
   /* variables */
   double* Y[64];
   /* random variables */
   double *epsilon;
   /* size of Yt Yt1 ... Ytp epsi */
   int     dim;
   /* from t to t-p */
   int     p;

   /* parameters 
    * the size is num_para */
   double  parameters[128];
   /* if active_marks[0] = 0, then parameters[0] has been given and will not be estimated 
    * the size is num_para */
   int     active_marks[128];
   int     num_para;
   /* if num_para = 3, active_marks[0] = 0, active_marks[1] = 1 active_marks[0] = 2, 
    * then num_type = 2. parameters[0] will not be estimated, and 
    * parameters[1], parameters[2] will be estimated iteratively */
   int     num_type;
   void  (*function)(double* Y[], double *parameters, double *epsilon, int flag);
} model_Variables;
typedef struct model_Variables_struct* MODEL_Variables;
/* use Yt1 ... Ytp, epsi and parameters to get Yt */
typedef void (*MODEL_Function)(double* Y[], double *parameters, double *epsilon, int flag);


#define MODEL_VariablesGetY(variables, idx)          ((variables) -> Y[idx])
#define MODEL_VariablesGetP(variables)               ((variables) -> p)
#define MODEL_VariablesGetDim(variables)             ((variables) -> dim)
#define MODEL_VariablesGetFunction(variables)        ((variables) -> function)

#define MODEL_VariablesGetParameters(variables)      ((variables) -> parameters)
#define MODEL_VariablesGetNumParameters(variables)   ((variables) -> num_para)
#define MODEL_VariablesGetEpsilon(variables)         ((variables) -> epsilon)
#define MODEL_VariablesGetActiveMarks(variables)     ((variables) -> active_marks)

void MODEL_VariablesCreate (MODEL_Variables* variables, int dim, int p, int num_para, MODEL_Function function);
void MODEL_VariablesDestroy(MODEL_Variables* variables);
void MODEL_VariablesSetActiveParameters(MODEL_Variables variables, double *real_para, int num_type, int *active_marks);
void MODEL_VariablesPrint(MODEL_Variables variables);
#endif
