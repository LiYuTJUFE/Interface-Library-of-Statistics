/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2018 03:42:04 PM
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
#include "model.h"
#include "time.h"


#define DIM       3

int main ( int argc, char *argv[] )
{
   int i, k;
   /* stored by col */
   double x[DIM] = {1, 2, 3};
   double y[DIM] = {3, 2, 1};
   double A[DIM*DIM] = {5, 2, 3,
                        2, 3, 4, 
                        3, 4, 5};
   double B[DIM*DIM] = {1, 2, 3,
                        4, 5, 6, 
                        7, 8, 9};
   printf ( "x\n" );
   for (i = 0; i < DIM; ++i)
   {
      printf ( "%.3f\n", x[i] );
   }
   printf ( "y\n" );
   for (i = 0; i < DIM; ++i)
   {
      printf ( "%.3f\n", y[i] );
   }
   printf ( "B\n" );
   for (i = 0; i < DIM; ++i)
   {
      for (k = 0; k < DIM; ++k)
      {
	 printf ( "%.3f  ", B[i+k*DIM] );
      }
      printf ( "\n" );
   }
   double a, b;
   a = 0.1; b = 0.1;
   mv(a, B, x, b, y, 'N', DIM, DIM);
   printf ( "y = %0.3f B x + %.3f y\n", a, b );
   for (i = 0; i < DIM; ++i)
   {
      printf ( "%.3f\n", y[i] );
   }

   cholesky(A, DIM, 'L');
   printf ( "A = L * L**T\n" );
   for (i = 0; i < DIM; ++i)
   {
      for (k = 0; k < DIM; ++k)
      {
	 printf ( "%.3f  ", A[i+k*DIM] );
      }
      printf ( "\n" );
   }
   return 0;
}				/* ----------  end of function main  ---------- */
