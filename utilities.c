/*
 * =====================================================================================
 *
 *       Filename:  utilities.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2018 02:52:54 PM
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
#include <math.h>

#include "utilities.h"

/** For BoxMuller */
#define MOD 2147483563
#define MUL 40014
#define QQQ 53668
#define RRR 12211


void copy(double *x, double *y, int size)
{
   for (int idx = 0; idx < size; ++idx)
      y[idx] = x[idx];
}
void cholesky(double *mat, int dim, char lu)
{
//   for (int idx = 0; idx < dim*dim; ++idx)
//      mat[idx] = 0;
   for (int idx = 0; idx < dim; ++idx)
      mat[idx*(dim+1)] = sqrt(mat[idx*(dim+1)]);
}
void inverse( double *mat, int dim, char lu)
{
//   for (int idx = 0; idx < dim*dim; ++idx)
//      mat[idx] = 0;
   for (int idx = 0; idx < dim; ++idx)
      mat[idx*(dim+1)] = 1 / mat[idx*(dim+1)];
}
/* y = a mat x + b y */
void mv(double a, double *mat, double *x, double b, double *y, char nt, int nrow, int ncol)
{
   int i, k;
   double z;
   for (i = 0; i < nrow; ++i)
   {
      z = 0;
      for (k = 0; k < ncol; ++k)
      {
	 z += mat[i+k*nrow]*x[k];
      }
      y[i] = a*z + b*y[i];
   }
}
void axpy(double a, double *x, double *y, int size)
{
   for (int i = 0; i < size; ++i)
   {
      y[i] = a*x[i] + y[i];
   }
}
void scal(double a, double *y, int size)
{
   for (int i = 0; i < size; ++i)
   {
      y[i] *= a;
   }
}
double dot(double *x, double *y, int size)
{
   double d = 0.0;
   for (int i = 0; i < size; ++i)
   {
      d += x[i]*y[i];
   }
   return d;
}

void SetMultivariateNormalRandomValues(double *random_values, double *mu, double *sigma, int dim, int num)
{
   double *chol_L = (double*)calloc(dim*dim, sizeof(double));
   copy(sigma, chol_L, dim*dim);
   /* sigma = L L' */
   cholesky( chol_L, dim, 'L');
   /* L^{-1} */
//   inverse( chol_L, dim, 'L');
//   printf ( "chol_L %e, %e, %e, %e\n", chol_L[0] , chol_L[4], chol_L[8],  chol_L[12]);
//   printf ( "chol_L %e, %e, %e, %e\n", chol_L[1] , chol_L[5], chol_L[9],  chol_L[13]);
//   printf ( "chol_L %e, %e, %e, %e\n", chol_L[2] , chol_L[6], chol_L[10], chol_L[14]);
//   printf ( "chol_L %e, %e, %e, %e\n", chol_L[3] , chol_L[7], chol_L[11], chol_L[15]);

   int size = num*dim/2;
   /* standardized normal random values */
   BoxMuller(random_values, random_values+size, 0, 1, size);

//   printf ( "size = %d, random_values[size] = %e\n", size, random_values[size] );
   if ( num*dim-2*size > 0 )
   {
      BoxMuller(random_values+2*size, NULL, 0, 1, num*dim-2*size);
   }
//   printf ( "size = %d, num*dim-2*size = %d\n", size, num*dim-2*size);
   
   double *tmp = (double *)calloc(dim, sizeof(double));
   
   double *ptr = random_values;
   for (int idx = 0; idx < num; ++idx)
   {
      /* tmp = L prt */
      mv(1.0, chol_L, ptr, 0.0, tmp, 'n', dim, dim);
//      printf ( "ptr = %e, %e, %e, %e\n", ptr[0], ptr[1], ptr[2], ptr[3] );
//      printf ( "tmp = %e, %e, %e, %e\n", tmp[0], tmp[1], tmp[2], tmp[3] );
      /* tmp = mu + tmp */
      axpy(1.0, mu, tmp, dim);
      /* ptr = tmp */
      copy(tmp, ptr, dim);
      ptr += dim;
   }
   free(chol_L);
   free(tmp);
}
/**
 * @brief 生成两个长度为num的相互独立的服从N(mu, sigma)的随机序列
 *
 * @param normal_x
 * @param normal_y
 * @param mu     期望
 * @param sigma  方差
 * @param num    个数
 *
 * @return 
 * @author Li Yu,  liyu@lsec.cc.ac.cn
 * @date 08/08/2014
 */
void BoxMuller(double *normal_x, double *normal_y, double mu, double sigma, int num)
{
   int flag = 0;
   if (normal_y==NULL)
   {
      normal_y = (double*)calloc(num, sizeof(double));
      flag = 1;
   }
   int i, k;
   double R, V;
   normal_x[0] = rand();
   k = normal_x[0]/QQQ;
   normal_x[0] = MUL*(normal_x[0]-k*QQQ)-k*RRR;
   if (normal_x[0]<0) normal_x[0] += MOD;
   for (i = 0; i < num-1; ++i )
   {
      k = normal_x[i]/QQQ;
      normal_x[i+1] = MUL*(normal_x[i]-k*QQQ)-k*RRR;
      if (normal_x[i+1]<0) normal_x[i+1] += MOD;
   }   /* -----  end for  ----- */
   k = normal_x[i]/QQQ;
   normal_y[0] = MUL*(normal_x[i]-k*QQQ)-k*RRR;
   if (normal_y[0]<0) normal_y[0] += MOD;
   for (i = 0; i < num-1; ++i )
   {
      k = normal_y[i]/QQQ;
      normal_y[i+1] = MUL*(normal_y[i]-k*QQQ)-k*RRR;
      if (normal_y[i+1]<0) normal_y[i+1] += MOD;
   }   /* -----  end for  ----- */
   for (i = 0; i < num; ++i )
   {
      normal_x[i] /= MOD;
      normal_y[i] /= MOD;
   }
   for (i = 0; i < num; ++i )
   {
      R = -2*log(normal_x[i]);
      R = sqrt(R);
      V = 2*M_PI*normal_y[i];
      normal_x[i] = mu + R*cos(V)*sigma;
      normal_y[i] = mu + R*sin(V)*sigma;
   }
   if (flag == 1)
   {
      free(normal_y);
   }
}

 
