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
#include "model.h"
#include "time.h"


#define DIM       4
#define P         2
#define NUM_PARA 30
#define T        100

int main ( int argc, char *argv[] )
{
   int    active_marks[NUM_PARA];
   double real_para[NUM_PARA], init_para[NUM_PARA];
   /* 多元正态分布的期望向量和协方差矩阵 */
   double mu[DIM] = {0, 0, 0, 0};
   double sigma[DIM*DIM] = {.1, 0, 0, 0, 
                            0, .1, 0, 0, 
			    0, 0, .1, 0, 
			    0, 0, 0, .1};

   /* 统计模型的框架结构 */
   MODEL_Variables variables;
   /* 真实数据 模拟数据 残量数据 随机数据 初始数据 */
   MODEL_Data      real_data, simu_data;
   MODEL_Data      resi_data, epsi_data;
   MODEL_Data      init_data;

   MODEL_VariablesCreate (&variables, DIM, P, NUM_PARA, vlstar);
   MODEL_DataCreate (&real_data, T, DIM);
   MODEL_DataCreate (&simu_data, T, DIM);
   MODEL_DataCreate (&resi_data, T, DIM);
   MODEL_DataCreate (&epsi_data, T, DIM);
   MODEL_DataCreate (&init_data, P, DIM);

   /* 设定统计模型中的参数 */
   setpara(real_para, 1);
   MODEL_VariablesSetActiveParameters(variables, real_para, -1, NULL);
   /* 生成多元正态分布随机向量 */
   srand(time(NULL));
   MODEL_DataSetMultivariateNormalRandomValues(epsi_data, mu, sigma);
   /* 以当前给定参数和随机向量生成数据 */
   MODEL_DataSetRandomValues(init_data);
   MODEL_Simulation(real_data, variables, init_data, epsi_data, 1);
//   MODEL_DataPrint(real_data);
   MODEL_Simulation(simu_data, variables, init_data, epsi_data, 0);
   double res = MODEL_EvalResiduals(resi_data, real_data, simu_data, variables);
//   MODEL_DataPrint(resi_data);
   printf ( "res = %e\n", res );
//   MODEL_DataPrint(epsi_data);

//   MODEL_DataPrint(real_data);
   /* 设定统计模型中的初始参数 */
   setpara(init_para, 1);
   MODEL_VariablesSetActiveParameters(variables, init_para, -1, NULL);
   /* 给定数据估计模型参数并返回残差数据 */
   MODEL_VariablesPrint(variables);
   MODEL_Estimation(real_data, simu_data, resi_data, variables, argc, argv);
//   MODEL_DataPrint(real_data);
//   MODEL_DataPrint(simu_data);

   MODEL_VariablesPrint(variables);

   res = 0;
   for(int i = 0; i < NUM_PARA; ++i)
   {
      res += variables->parameters[i] - init_para[i];
      printf ( "%f\n", variables->parameters[i] - init_para[i] );
   }
   printf ( "res of para = %f \n", res );

   MODEL_DataDestroy(&real_data);
   MODEL_DataDestroy(&simu_data);
   MODEL_DataDestroy(&resi_data);
   MODEL_DataDestroy(&epsi_data);
   MODEL_DataDestroy(&init_data);

   MODEL_VariablesDestroy(&variables);

   return 0;
}				/* ----------  end of function main  ---------- */
