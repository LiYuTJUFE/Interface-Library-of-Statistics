/*
 * =====================================================================================
 *
 *       Filename:  app_mlr.c
 *
 *    Description:  多元线性回归
 *
 *        Version:  1.0
 *        Created:  11/06/2018 10:26:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <math.h>
#include "app_mlr.h"

/* if flag == 1, then epsilon should be used
 * 
 * Yt = beta0 + beta1' Xt + epsilon
 *
 * */
void mlr(double* Y[], double *parameters, double *epsilon, int flag)
{
   double *Yt, *Xt, *beta0, *beta1, *epst, tmp;
   int size  = 3;

   Yt        = Y[0];
   Xt        = Yt+1;

   beta0     = parameters;
   beta1     = beta0+1;

   /* Yt = beta0 + beta1' Xt */
   tmp       = dot(beta1, Xt, size);
   Yt[0]     = beta0[0] + tmp;

   if (flag)
   {
      epst   = epsilon;
      Yt[0] += epst[0];
   }
}
/* The size of parameters 
 *   beta0   1   
 *   beta1   3
 * */
void setpara(double *parameters, int flag)
{
   if (flag)
   {
      double *beta0, *beta1;
      beta0     = parameters;
      beta1     = beta0+1;

      beta0[0]  = 0.01;
      beta1[0]  = 1.0;
      beta1[1]  = 1.0;
      beta1[2]  = 1.0;
   }
   else 
   {
      for (int i = 0; i < 4; ++i)
      {
	 *parameters = 0.0; ++parameters;
      }
   }
}
