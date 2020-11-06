#include <iostream>
#include <string>

using namespace std;

#define MAX_INPUT_LENGTH	20
#define WAVE_RATE	0.25

//1. �ṹ�����Ͷ���
typedef struct
{
	float LastP;	//�ϴι���Э���� ��ʼ��ֵΪ0.02
	float Now_P;	//��ǰ����Э���� ��ʼ��ֵΪ0
	float out;		//�������˲������ ��ʼ��ֵΪ0
	float Kg;		//���������� ��ʼ��ֵΪ0
	float Q;		//��������Э���� ��ʼ��ֵΪ0.001
	float R;		//�۲�����Э���� ��ʼ��ֵΪ0.543
}KFP;				//Kalman Filter Parameter

//2. �Ը߶�Ϊ�� ���忨�����ṹ�岢��ʼ������
KFP KFP_height = { 
	0.02,
	0,
	0,
	0,
	0.001,
	0.001 };

/**
 *�������˲���
 *@param KFP *kfp �������ṹ�����
 *   float input ��Ҫ�˲��Ĳ����Ĳ���ֵ�����������Ĳɼ�ֵ��
 *@return �˲���Ĳ���������ֵ��
 */
float kalmanFilter(KFP *kfp, float input)
{
	//Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����
	kfp->Now_P = kfp->LastP + kfp->Q;
	//���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���
	kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
	//��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
	kfp->out = kfp->out + kfp->Kg * (input - kfp->out);//��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
	//����Э�����: ���ε�ϵͳЭ����� kfp->LastP Ϊ��һ������׼����
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
 *���ÿ������˲��� ʵ��
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