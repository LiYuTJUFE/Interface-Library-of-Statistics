/*
 * =====================================================================================
 *
 *       Filename:  model.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/06/2018 08:54:29 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  _MODEL_H_
#define  _MODEL_H_

#include <petsctao.h>
#include "utilities.h"
#include "variables.h"
#include "data.h"
#include "app.h"

typedef struct model_Statistics_struct
{
   double p_value;
   int    LM;
} model_Statistics;
typedef struct model_Statistics_struct* MODEL_Statistics;
int MODEL_StatisticsCreate (MODEL_Statistics statistics);//, Probability_Density_Function);
int MODEL_StatisticsDestroy(MODEL_Statistics statistics);

/* based on variables and function to get data */
void MODEL_Simulation(MODEL_Data data, MODEL_Variables variables, MODEL_Data init_data, MODEL_Data epsi_data, int flag);
/* based on real_data, variables to get para in variables and resi_data */
PetscErrorCode MODEL_Estimation(MODEL_Data real_data, MODEL_Data simu_data, MODEL_Data resi_data, 
      MODEL_Variables variables, int argc,char **argv);
/* based on data, variables and function to get residuals */
double MODEL_EvalResiduals(MODEL_Data resi_data, MODEL_Data real_data, MODEL_Data simu_data, MODEL_Variables variables);
/* based on residuals to get statistics */
void MODEL_StatisTest(MODEL_Statistics statistics, MODEL_Data residuals);

#endif
