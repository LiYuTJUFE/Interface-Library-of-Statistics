/*
 * =====================================================================================
 *
 *       Filename:  vlstar.c
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
#include "time.h"
#include "model.h"
#include "app_vlstar_p2r1.h"


#define DIM       2
#define P         1
#define NUM_PARA  10
#define T         300

int main ( int argc, char *argv[] )
{
   int    active_marks[NUM_PARA];
   double real_para[NUM_PARA], init_para[NUM_PARA];
   /* 多元正态分布的期望向量和协方差矩阵 */
   double mu[DIM] = {0, 0};
   double sigma[DIM*DIM] = {1.0, 0.5,
                            0.5, 1.0};

   /* 创建统计模型的框架结构 */
   MODEL_Variables variables;
   MODEL_VariablesCreate (&variables, DIM, P, NUM_PARA, vlstar_p2r1);
   /* 真实数据 模拟数据 残量数据 随机数据 */
   MODEL_Data      real_data, simu_data;
   MODEL_Data      resi_data, epsi_data;
   MODEL_DataCreate (&real_data, T, DIM);
   MODEL_DataCreate (&simu_data, T, DIM);
   MODEL_DataCreate (&resi_data, T, DIM);
   MODEL_DataCreate (&epsi_data, T, DIM);

   /* 设定统计模型中的参数 */
   setpara_vlstar_p2r1(real_para, 1);
   MODEL_VariablesSetActiveParameters(variables, real_para, 1, NULL);
   /* 生成多元正态分布随机向量 */
   srand(314);
//   srand(time(NULL));
   MODEL_DataSetMultivariateNormalRandomValues(epsi_data, mu, sigma);
   /* 以当前给定参数和随机向量生成数据 */
   MODEL_DataSetRandomValues(real_data);
#if 1
   real_data->Y[0] = 1;
   real_data->Y[1] = 1;
#endif
   MODEL_Simulation(real_data, variables, real_data, epsi_data, 1);
//   MODEL_DataPrint(real_data);
   MODEL_Simulation(simu_data, variables, real_data, epsi_data, 0);
//   MODEL_DataPrint(simu_data);
   double res = MODEL_EvalResiduals(resi_data, real_data, simu_data, variables);
   printf ( "res = %f\n", res );
//   MODEL_DataPrint(epsi_data);
//   MODEL_DataPrint(resi_data);
   MODEL_VariablesPrint(variables);

   double est_sigma[DIM*DIM];
   MODEL_TestSigma(epsi_data, variables, est_sigma);
#if 1
   /* 设定统计模型中的初始参数 */
   setpara_vlstar_p2r1(init_para, 0);
   int num_type = 2;
   int idx;
   for (idx = 0; idx < NUM_PARA; ++idx)
   {
      active_marks[idx] = 1;
   }
   active_marks[NUM_PARA-2] = 2;
   active_marks[NUM_PARA-1] = 2;
   MODEL_VariablesSetActiveParameters(variables, init_para, num_type, active_marks);
   /* 给定数据估计模型参数并返回残差数据 */
   MODEL_Estimation(real_data, simu_data, resi_data, variables, argc, argv);
//   MODEL_DataPrint(real_data);
   MODEL_DataPrint(simu_data);
   MODEL_VariablesPrint(variables);

//   MODEL_DataPrint(resi_data);
   MODEL_TestSigma(resi_data, variables, est_sigma);

   res = 0;
   for(idx = 0; idx < NUM_PARA; ++idx)
   {
      res += fabs(variables->parameters[idx] - real_para[idx]);
//      printf ( "%f\n", variables->parameters[i] - real_para[i] );
   }
   printf ( "res of para = %e \n", res );
#endif

   MODEL_DataDestroy(&real_data);
   MODEL_DataDestroy(&simu_data);
   MODEL_DataDestroy(&resi_data);
   MODEL_DataDestroy(&epsi_data);
   MODEL_VariablesDestroy(&variables);

   return 0;
}				/* ----------  end of function main  ---------- */
