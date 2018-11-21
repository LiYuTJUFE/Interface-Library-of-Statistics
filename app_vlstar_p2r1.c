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
#include "app_vlstar_p2r1.h"

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
void vlstar_p2r1(double* Y[], double *parameters, double *epsilon, int flag)
{
   double  *Yt, *Yt1, st;

   Yt      = Y[0];
   Yt1     = Y[1];
   st      = 0.5*(Yt1[0]+Yt1[1]);
//   printf ( "Yt  = %0.2e, %0.2e\n", Yt[0], Yt[1]);
//   printf ( "Yt1 = %0.2e, %0.2e\n", Yt1[0], Yt1[1]);
//   printf ( "st  = %0.2e\n", st );

   double  *para, *Phi1, *Psi1, *gamma, *c, g;
   para    = parameters;
   Phi1    = para;
   Psi1    = Phi1+4;
   gamma   = Psi1+4;
   c       = gamma+1;

   /* Yt = Phi1 Yt1 + Psi1 Yt1 G(st, gamma, c) */
   mv (1.0, Psi1, Yt1, 0.0, Yt, 'N', 2, 2);
   /* g = 1 / (1+exp(-gamma(st-c))) */
   g = sigmoid(&st, gamma, c, 1);
   scal (g, Yt, 2);
   mv (1.0, Phi1, Yt1, 1.0, Yt, 'N', 2, 2);

   if (flag)
   {
      double  *epst;
      epst    = epsilon;
      axpy(1.0, epst, Yt, 2);
   }
}
/* The size of parameters
 *   Phi1    4*4     Psi1    4*4
 *   gamma   1       c       1
 * */
void setpara_vlstar_p2r1(double *parameters, int flag)
{
   int i;
   if (flag)
   {
      double  *para, *Phi1, *Psi1, *gamma, *c;
      para    = parameters;
      Phi1    = para;
      Psi1    = Phi1+4;
      gamma   = Psi1+4;
      c       = gamma+1;
      
      /* Phi1 */
      Phi1[0] = 0.5; Phi1[2] = 0.0;
      Phi1[1] = 0.0; Phi1[3] = 0.5;
      /* Psi1 */
      Psi1[0] =-0.5; Psi1[2] = 0.5;
      Psi1[1] = 1.0; Psi1[3] =-1.0;
      /* gamma c */
      gamma[0] = 10.0; c[0] = 0.0;
   }
   else 
   {
      for (i = 0; i < 10; ++i)
      {
	 *parameters = 1.0; ++parameters;
      }
   }
}
