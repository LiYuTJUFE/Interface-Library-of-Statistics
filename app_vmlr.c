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
#include "app_vmlr.h"

/* if flag == 1, then epsilon should be used
 * 
 * Yt = beta0 + beta1 Xt + epsilon
 *
 * the size of Yt beta0 beta1 epsilon is 
 *  2*1   2*1   2*4   4*1   2*1
 *
 * */
void vmlr(double* Y[], double *parameters, double *epsilon, int flag)
{
   double *Yt, *Xt, *beta0, *beta1, *epst;

   Yt        = Y[0];
   Xt        = Yt+2;

   beta0     = parameters;
   beta1     = beta0+2;

   /* Yt = beta0 + beta1 Xt */
   mv(1.0, beta1, Xt, 0.0, Yt, 'N', 2, 4);
   axpy(1.0, beta0, Yt, 2);

   if (flag)
   {
      epst   = epsilon;
      axpy(1.0, epst, Yt, 2);
   }
}
/* The size of parameters 
 *   beta0   2   
 *   beta1   2*4
 * */
void setpara_vmlr(double *parameters, int flag)
{
   if (flag)
   {
      double *beta0, *beta1;
      beta0     = parameters;
      beta1     = beta0+2;

      beta0[0]  = 0.1;
      beta0[1]  = 0.1;
      beta1[0]  = 1.0; beta1[2]  = 0.0; beta1[4]  = 1.0; beta1[6]  = 0.0;
      beta1[1]  = 0.0; beta1[3]  = 1.0; beta1[5]  = 0.0; beta1[7]  = 1.0;
   }
   else 
   {
      for (int i = 0; i < 10; ++i)
      {
	 *parameters = 0.0; ++parameters;
      }
   }
}
