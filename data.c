/*
 * =====================================================================================
 *
 *       Filename:  data.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2018 07:59:56 PM
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

#include "data.h"
#include "utilities.h"


void MODEL_DataCreate (MODEL_Data* data, int T, int dim)
{
   *data = malloc(sizeof(model_Data));

   MODEL_DataGetT(*data)    = T;
   MODEL_DataGetDim(*data)  = dim;
   (*data)->Y = (double*)calloc(T*dim, sizeof(double));
}
void MODEL_DataDestroy(MODEL_Data* data)
{
   free((*data)->Y);
   free(*data);
   *data = NULL;
}
void MODEL_DataCopy(MODEL_Data s_data, MODEL_Data d_data)
{
   double *s_ptr = MODEL_DataGetY(s_data, 0);
   double *d_ptr = MODEL_DataGetY(d_data, 0);
   int size = MODEL_DataGetT(d_data)*MODEL_DataGetDim(d_data);
   copy(s_ptr, d_ptr, size);
}

void MODEL_DataSetZeros(MODEL_Data data)
{
   double *ptr = MODEL_DataGetY(data, 0);
   for (int idx = 0; idx < MODEL_DataGetT(data)*MODEL_DataGetDim(data); ++idx )
   {
      *ptr = 0.0; ++ptr;
   }
}
void MODEL_DataSetRandomValues(MODEL_Data data)
{
   double *ptr = MODEL_DataGetY(data, 0);
   for (int idx = 0; idx < MODEL_DataGetT(data)*MODEL_DataGetDim(data); ++idx )
   {
      *ptr = ((double)rand())/((double)(RAND_MAX)); ++ptr;
   }
}
void MODEL_DataSetMultivariateNormalRandomValues(MODEL_Data data, double *mu, double *sigma)
{
   SetMultivariateNormalRandomValues(MODEL_DataGetY(data, 0), mu, sigma, MODEL_DataGetDim(data), MODEL_DataGetT(data));
}
void MODEL_DataPrint(MODEL_Data data)
{
   double *ptr;
   printf ( "--------------------------------------------\n" );
   for (int idx = 0; idx < MODEL_DataGetT(data); ++idx )
   {
      ptr = MODEL_DataGetY(data, idx);
      for (int k = 0; k < MODEL_DataGetDim(data); ++k)
      {
	 printf ( "%0.2e\t", ptr[k] );
      }
      printf ( "\n" );
   }
   printf ( "--------------------------------------------\n" );
}


double* MODEL_DataGetY(MODEL_Data data, int idx)
{
   return (((data)-> Y)+idx*((data)->dim));
}
