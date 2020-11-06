#include <iostream>
#include <string>

using namespace std;

#define MAX_INPUT_LENGTH	20
#define WAVE_RATE	0.25

//1. 结构体类型定义
typedef struct
{
	float LastP;	//上次估算协方差 初始化值为0.02
	float Now_P;	//当前估算协方差 初始化值为0
	float out;		//卡尔曼滤波器输出 初始化值为0
	float Kg;		//卡尔曼增益 初始化值为0
	float Q;		//过程噪声协方差 初始化值为0.001
	float R;		//观测噪声协方差 初始化值为0.543
}KFP;				//Kalman Filter Parameter

//2. 以高度为例 定义卡尔曼结构体并初始化参数
KFP KFP_height = { 
	0.02,
	0,
	0,
	0,
	0.001,
	0.001 };

/**
 *卡尔曼滤波器
 *@param KFP *kfp 卡尔曼结构体参数
 *   float input 需要滤波的参数的测量值（即传感器的采集值）
 *@return 滤波后的参数（最优值）
 */
float kalmanFilter(KFP *kfp, float input)
{
	//预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
	kfp->Now_P = kfp->LastP + kfp->Q;
	//卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
	kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
	//更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
	kfp->out = kfp->out + kfp->Kg * (input - kfp->out);//因为这一次的预测值就是上一次的输出值
	//更新协方差方程: 本次的系统协方差付给 kfp->LastP 为下一次运算准备。
	kfp->LastP = (1 - kfp->Kg) * kfp->Now_P;
	return kfp->out;
}

static void dumpData(const char *str, float *input, int length)
{
	int i;
	printf("----    DUMP %s    ----\n", str);
	printf("Indx: ");
	for (i = 0; i < length; i++)
		printf("%5d ", i);
	printf("\nData: ");
	for (i = 0; i < length; i++)
		printf("%5.2f ", input[i]);
	printf("\n");
}

static void initInputData(float *input)
{
	int i;

	for (i = 0; i < MAX_INPUT_LENGTH; i++)
	{
		input[i] = (float)i * ((i % 2 == 0) ? (1 + WAVE_RATE) : (1 - WAVE_RATE));
	}
}

/**
 *调用卡尔曼滤波器 实践
 */
int main()
{
	int i;
	int kalman_height = 0;
	float dataMeasure[MAX_INPUT_LENGTH];
	float dataOutput[MAX_INPUT_LENGTH];
	initInputData(dataMeasure);
	dumpData("dataMeasure", dataMeasure, MAX_INPUT_LENGTH);

	for (i = 0; i < MAX_INPUT_LENGTH; i++)
	{
		dataOutput[i] = kalmanFilter(&KFP_height, dataMeasure[i]);
	}

	dumpData("dataOutput", dataOutput, MAX_INPUT_LENGTH);

	system("pause");
	return true;
}