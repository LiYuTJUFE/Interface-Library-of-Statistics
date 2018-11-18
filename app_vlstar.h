/*
 * =====================================================================================
 *
 *       Filename:  variables.h
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


#ifndef  _APP_VLSTAR_H_
#define  _APP_VLSTAR_H_

#include "variables.h"
#include "utilities.h"

void setpara_vlstar(double *parameters, int flag);
double sigmoid(double *s, double *gamma, double *c, int size);
void vlstar(double* Y[], double *parameters, double *epsilon, int flag);
#endif
