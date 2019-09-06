#include "MySort.h"
#include <Windows.h>

using namespace std;
#define SWAP(X, Y) (X)=(X)+(Y);(Y)=(X)-(Y);(X)=(X)-(Y)



MySort::MySort(int* p, int cap)
{
	m_iCap = cap;
	m_ipArray = new int[m_iCap];
	memset(m_ipArray, 0, m_iCap);

	memcpy(m_ipArray, p, sizeof(m_ipArray[0]) * m_iCap);
	showArray(m_ipArray, m_iCap);
}

MySort::~MySort()
{
	delete[]m_ipArray;
}


bool MySort::showArray(int* p, int len)
{
	if (len <= 0)
	{
		cout << "Please input correct length" << endl;
		return false;
	}

	for (int i = 0; i < len; i++)
	{
		cout << p[i] << " ";
	}
	cout << endl;

	return true;
}

bool MySort::bubbleSort(UP_DOWN Up_Down)
{
	int i = 0, j = 0;
	DWORD count = 0;
	bool isSorted = true;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pBubble = new int[m_iCap];
	if (NULL == pBubble)
	{
		cout << "New Fail!" << endl;
		return false;
	}

	memset(pBubble, 0, sizeof(pBubble[0]) * m_iCap);
	memcpy(pBubble, m_ipArray, sizeof(pBubble[0]) * m_iCap);

	for (i = 1; i < m_iCap; i++)
	{
		for (j = 0; j < m_iCap - i; j++)
		{
			count++;

			if (UP == Up_Down && pBubble[j] > pBubble[j + 1])
			{
				SWAP(pBubble[j], pBubble[j + 1]);
				isSorted = false;
			}
			else if (DOWN == Up_Down && pBubble[j] < pBubble[j + 1])
			{
				SWAP(pBubble[j], pBubble[j + 1]);
				isSorted = false;
			}
			else
			{
				continue;
			}
		}
		if (isSorted)
		{
			break;
		}
	}

	showArray(pBubble, m_iCap);
	printf("bubbleSort takes [%lu] ms when n = [%d] \n", count, m_iCap);

	delete[]pBubble;
	return true;
}
