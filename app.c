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
#include "app.h"

double sigmoid(double *s, double *gamma, double *c, int size)
{
   double g;
   g = dot(gamma, s, size);
   g = g - dot(gamma, c, size);
   g = 1.0 / (1+exp(-g));
   return g;
}
/* if flag == 1, then epsilon should be used
 * The size of Y Y1t Y2t are 4, 2, 2  
 *   pi      2*2     phi1_s  2*4 
 *   phi2_s  2*4     psi2_s  2*4
 *   gamma   1       c       1
 * */
void vlstar(double* Y[], double *parameters, double *epsilon, int flag)
{
   double *Yt, *Yt1, *Yt2, st, *Y1t, *Y2t, *para, *epst, *eps1t, *eps2t;

   Yt      = Y[0];
   Yt1     = Y[1];
   Yt2     = Y[2];
   Y1t     = Yt;
   Y2t     = Yt+2;
   st      = 0.25*(Yt2[0]+Yt2[1]+Yt2[2]+Yt2[3]);
   para    = parameters;


//   printf ( "Yt  = %p\n", Yt); 
//   printf ( "Yt1 = %p\n", Yt1); 
//   printf ( "Yt2 = %p\n", Yt2); 


   double *pi, *phi1_s, *phi2_s, *psi2_s, *gamma, *c, g;
   pi      = para;
   phi1_s  = pi+4;
   phi2_s  = phi1_s+8;
   psi2_s  = phi2_s+8;
   gamma   = psi2_s+8;
   c       = gamma+1;

   /* Y1t = pi'    * Y2t + phi1_s * Yt1                   */
   mv (1.0, pi,     Y2t, 0.0, Y1t, 'T', 2, 2);
   mv (1.0, phi1_s, Yt1, 1.0, Y1t, 'N', 2, 4);
   /* Y2t = phi2_s * Yt1 + psi2_s * Yt1 * G(st, gamma, c) */
   mv (1.0, psi2_s, Yt1, 0.0, Y2t, 'N', 2, 4);
   /* g = 1 / (1+exp(-gamma(st-c))) */
   g = sigmoid(&st, gamma, c, 1);
   scal (g, Y2t, 2);
   mv (1.0, phi2_s, Yt1, 1.0, Y2t, 'N', 2, 4);

//   printf ( "Yt1 = %e, %e\n", Yt1[0], Yt1[1] );
//   printf ( "Yt2 = %e, %e\n", Yt2[0], Yt2[1] );

   if (flag)
   {
      epst    = epsilon;
      eps1t   = epst;
      eps2t   = eps1t+2;
      axpy(1.0, eps1t, Y1t, 2);
      axpy(1.0, eps2t, Y2t, 2);
   }
}
/* The size of parameters
 *   pi      2*2     phi1_s  2*4 
 *   phi2_s  2*4     psi2_s  2*4
 *   gamma   1       c       1
 * */
void setpara(double *para, int flag)
{
   if (flag)
   {
      double *pi, *phi1_s, *phi2_s, *psi2_s, *gamma, *c;
      pi     = para;
      phi1_s = pi+4;
      phi2_s = phi1_s+8;
      psi2_s = phi2_s+8;
      gamma  = psi2_s+8;
      c      = gamma+1;

      /* pi */
      pi[0] = 0.1; pi[2] = 0.0;
      pi[1] = 0.0; pi[3] =-0.1;
      /* phi1_s */
      phi1_s[0] = 0.1; phi1_s[2] = 0.0; phi1_s[4] = 0.0; phi1_s[6] = 0.1;
      phi1_s[1] = 0.0; phi1_s[3] =-0.1; phi1_s[5] =-0.1; phi1_s[7] = 0.0;
      /* phi2_s */
      phi2_s[0] = 0.1; phi2_s[2] = 0.0; phi2_s[4] = 0.0; phi2_s[6] = 0.1;
      phi2_s[1] = 0.0; phi2_s[3] =-0.1; phi2_s[5] =-0.1; phi2_s[7] = 0.0;
      /* psi2_s */
      psi2_s[0] = 0.1; psi2_s[2] = 0.0; psi2_s[4] = 0.0; psi2_s[6] = 0.1;
      psi2_s[1] = 0.0; psi2_s[3] =-0.1; psi2_s[5] =-0.1; psi2_s[7] = 0.0;
      /* gamma c */
      gamma[0] = 0.1; c[0] = 0.1;
   }
   else 
   {
      for (int i = 0; i < 30; ++i)
      {
	 *para = 0.0; ++para;
      }
   }
}
