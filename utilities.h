/*
 * =====================================================================================
 *
 *       Filename:  utilities.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2018 07:35:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  _UTILITIES_H_
#define  _UTILITIES_H_


void copy(double *x, double *y, int size);
void cholesky(double *chol, int dim, char lu);
void mv(double a, double *mat, double *x, double b, double *y, char nt, int nrow, int ncol);
void axpy(double a, double *x, double *y, int size);
void scal(double a, double *y, int size);
double dot(double *x, double *y, int size);


void SetMultivariateNormalRandomValues(double *random_values, double *mu, double *sigma, int dim, int num);
void BoxMuller(double *normal_x, double *normal_y, double mu, double sigma, int num);


#endif
