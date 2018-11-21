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


#ifndef  _APP_VLSTAR_P2R1_H_
#define  _APP_VLSTAR_P2R1_H_

#include "variables.h"
#include "utilities.h"

void setpara_vlstar_p2r1(double *parameters, int flag);
double sigmoid(double *s, double *gamma, double *c, int size);
void vlstar_p2r1(double* Y[], double *parameters, double *epsilon, int flag);
#endif
