/*
 * =====================================================================================
 *
 *       Filename:  variables.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2018 09:05:21 PM
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

#include "variables.h"

void MODEL_VariablesCreate (MODEL_Variables* variables, int dim, int p, int num_para, MODEL_Function function)
{
   *variables = malloc(sizeof(model_Variables)); 
   MODEL_VariablesGetDim(*variables)              = dim;
   MODEL_VariablesGetP(*variables)                = p;
   MODEL_VariablesGetNumParameters(*variables)    = num_para;
   MODEL_VariablesGetFunction(*variables)         = function;
}
void MODEL_VariablesDestroy(MODEL_Variables* variables)
{
   free(*variables);
   *variables = NULL;
}
void MODEL_VariablesSetActiveParameters(MODEL_Variables variables, double *real_para, int num_type, int *active_marks)
{
   double *ptr = MODEL_VariablesGetParameters(variables);
   int  *marks = MODEL_VariablesGetActiveMarks(variables);
   if (active_marks == NULL)
   {
      variables->num_type = 1;
      for (int idx = 0; idx < MODEL_VariablesGetNumParameters(variables); ++idx)
      {
	 ptr[idx]   = real_para[idx];
	 marks[idx] = 1;
      }
   }
   else {
      variables->num_type = num_type;
      for (int idx = 0; idx < MODEL_VariablesGetNumParameters(variables); ++idx)
      {
	 ptr[idx]   = real_para[idx];
	 marks[idx] = active_marks[idx];
      }
   }
}

void MODEL_VariablesPrint(MODEL_Variables variables)
{
   printf ( "Parameters\n" );
   for (int idx = 0; idx < MODEL_VariablesGetNumParameters(variables); ++idx)
   {
      printf ( "%0.2e  ", MODEL_VariablesGetParameters(variables)[idx] );
   }
   printf ( "\n" );
}
