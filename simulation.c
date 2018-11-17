/*
 * =====================================================================================
 *
 *       Filename:  simulation.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2018 11:29:07 AM
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
#include "model.h"

void MODEL_Simulation(MODEL_Data data, MODEL_Variables variables, MODEL_Data init_data, MODEL_Data epsi_data, int flag_eps)
{
   int p, tick, idx;
   p = MODEL_VariablesGetP(variables);  
//   assert(MODEL_DataGetT(init_data) == p || flag_eps);
   for (tick = 0; tick < p; ++tick)
   {
      copy(MODEL_DataGetY(init_data, tick), MODEL_DataGetY(data, tick), MODEL_VariablesGetDim(variables));
   }
   /* for regression */
   if (p == 0)
   {
      MODEL_DataCopy(init_data, data);
   }
   /* Yt = Y[0]  Yt1 = Y[1]  ...  Ytp = Y[p] */
   double **Y = variables->Y;
   for (tick = p; tick < MODEL_DataGetT(data); ++tick)
   {
      if (flag_eps) MODEL_VariablesGetEpsilon(variables) = MODEL_DataGetY(epsi_data, tick);
      for (idx = 0; idx <= p; ++idx)
      {
	 Y[idx] = MODEL_DataGetY(data, tick-idx);
//	 printf ( "Y[%d] = %p\n", idx, MODEL_DataGetY(data, tick-idx)); 
      }
      (MODEL_VariablesGetFunction(variables))(variables->Y, variables->parameters, variables->epsilon, flag_eps);
   }
}
