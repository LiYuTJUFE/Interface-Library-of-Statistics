/*
 * =====================================================================================
 *
 *       Filename:  app.c
 *
 *    Description:  
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
#include "app_vlstar0.h"

/* g = 1 / (1+exp(-gamma(s-c))) */
double sigmoid(double *s, double *gamma, double *c, int size)
{
   double g;
   g = dot(gamma, s, size);
   g = g - dot(gamma, c, size);
   g = 1.0 / (1+exp(-g));
   return g;
}
/* if flag == 1, then epsilon should be used
 *
 * Yt = Phi1 Yt1 + Psi1 Yt1 G(st, gamma, c) + epsilon
 *
 * The size of Yt Yt1 are 4, 4  
 *   Phi1    4*4     Psi1    4*4
 *   gamma   1       c       1
 * */
void vlstar0(double* Y[], double *parameters, double *epsilon, int flag)
{
   double  *Yt, *Yt1, *Yt2, st;

   Yt      = Y[0];
   Yt1     = Y[1];
   Yt2     = Y[2];
   st      = 0.25*(Yt2[0]+Yt2[1]+Yt2[2]+Yt2[3]);
//   printf ( "st = %0.2e\n", st );

   double  *para, *Phi1, *Psi1, *gamma, *c, g;
   para    = parameters;
   Phi1    = para;
   Psi1    = Phi1+16;
   gamma   = Psi1+16;
   c       = gamma+1;

   /* Yt = Phi1 Yt1 + Psi1 Yt1 G(st, gamma, c) */
   mv (1.0, Psi1, Yt1, 0.0, Yt, 'N', 4, 4);
   /* g = 1 / (1+exp(-gamma(st-c))) */
   g = sigmoid(&st, gamma, c, 1);
   scal (g, Yt, 4);
   mv (1.0, Phi1, Yt1, 1.0, Yt, 'N', 4, 4);

   if (flag)
   {
      double  *epst;
      epst    = epsilon;
      axpy(1.0, epst, Yt, 4);
   }
}
/* The size of parameters
 *   Phi1    4*4     Psi1    4*4
 *   gamma   1       c       1
 * */
void setpara_vlstar0(double *parameters, int flag)
{
   if (flag)
   {
      double  *para, *Phi1, *Psi1, *gamma, *c;
      para    = parameters;
      Phi1    = para;
      Psi1    = Phi1+16;
      gamma   = Psi1+16;
      c       = gamma+1;
      
      /* Phi1 */
      Phi1[0] = 0.8; Phi1[4] = 0.0; Phi1[8]  = 0.0; Phi1[12] = 0.0;
      Phi1[1] = 0.0; Phi1[5] = 0.8; Phi1[9]  = 0.0; Phi1[13] = 0.0;
      Phi1[2] = 0.0; Phi1[6] = 0.0; Phi1[10] = 0.8; Phi1[14] = 0.0;
      Phi1[3] = 0.0; Phi1[7] = 0.0; Phi1[11] = 0.0; Phi1[15] = 0.8;
      /* Psi1 */
      Psi1[0] =-1.0; Psi1[4] = 0.0; Psi1[8]  = 0.0; Psi1[12] = 0.0;
      Psi1[1] = 0.0; Psi1[5] =-1.0; Psi1[9]  = 0.0; Psi1[13] = 0.0;
      Psi1[2] = 0.0; Psi1[6] = 0.0; Psi1[10] =-1.0; Psi1[14] = 0.0;
      Psi1[3] = 0.0; Psi1[7] = 0.0; Psi1[11] = 0.0; Psi1[15] =-1.0;
      /* gamma c */
      gamma[0] = 10.0; c[0] = 0.0;
   }
   else 
   {
      for (int i = 0; i < 34; ++i)
      {
	 *parameters = 0.0; ++parameters;
      }
   }
}
