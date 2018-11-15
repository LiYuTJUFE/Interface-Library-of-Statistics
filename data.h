/*
 * =====================================================================================
 *
 *       Filename:  data.h
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


#ifndef  _DATA_H_
#define  _DATA_H_

#include "utilities.h"

typedef struct model_Data_struct
{
   /* storaged by column and size = T*dim
    * Y0[0]      Y1[0] ... Y{T-1}[0]  
    * Y0[1]      Y1[1] ... Y{T-1}[1]  
    * ...  
    * Y0[dim-1]  Y1[1] ... Y{T-1}[dim-1] */
   double *Y;
   /* period */
   int     T;
   /* dim of Y, which is leading dimension */
   int     dim;
   
} model_Data;
typedef struct model_Data_struct* MODEL_Data;

                
#define MODEL_DataGetT(data)                     ((data) -> T)
#define MODEL_DataGetDim(data)                   ((data) -> dim)
double* MODEL_DataGetY(MODEL_Data data, int idx);
void MODEL_DataCreate (MODEL_Data* data, int T, int dim);
void MODEL_DataDestroy(MODEL_Data* data);
void MODEL_DataSetZeros(MODEL_Data data);
void MODEL_DataSetRandomValues(MODEL_Data data);
void MODEL_DataSetMultivariateNormalRandomValues(MODEL_Data data, double *mu, double *sigma);
void MODEL_DataPrint(MODEL_Data data);

#endif
