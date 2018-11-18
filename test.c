/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2018 01:46:31 PM
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


/* C = alpha A B + beta C */
void dgemm_(char *transA, char *transB,
      int *m, int *n, int *k, 
      double *alpha, 
      double *A, int *ldA, 
      double *B, int *ldB, 
      double *beta, 
      double *C, int *ldC 
      );


/* 函数名需要修改，这里对计算 (1/T) \sum e_i e_i' */
void MODEL_TestSigma(MODEL_Data resi_data, MODEL_Variables variables, double *sigma)
{
   int    idx, T;
   char   transA, transB;
   int    m, n, k;
   double *A, *B, *C;
   int    ldA, ldB, ldC;
   double alpha, beta;

   transA = 'N'; transB = 'N';
   m   = MODEL_DataGetDim(resi_data);
   n   = m;
   k   = 1;
   T   = MODEL_DataGetT(resi_data);
   ldA = m; ldB = 1; ldC = m;
   C   = sigma;
   alpha = 1.0; beta = 1.0;

   for (idx = 0; idx < m*n; ++idx)
   {
      C[idx] = 0.0;
   }

   printf ( "------------------------------------------\n" );
   for (idx = 0; idx < T; ++idx)
   {
      A = MODEL_DataGetY(resi_data, idx);
      B = A;
      dgemm_(&transA, &transB, &m, &n, &k, 
	    &alpha, A, &ldA, B, &ldB, 
	    &beta, C, &ldC);
   }

   for (idx = 0; idx < m*n; ++idx)
   {
      C[idx] *= (1.0/T);
   }


   printf ( "--------------------------------------------" );
   for (idx = 0; idx < m*n; ++idx )
   {
      if (idx%m == 0) printf ( "\n" );
      printf ( "%0.2e\t", C[idx] );
   }
   printf ( "\n--------------------------------------------\n" );
}
