#include "MySort.h"
#include <Windows.h>

using namespace std;
#define SWAP(X, Y) (X)=(X)+(Y);(Y)=(X)-(Y);(X)=(X)-(Y)

MySort::MySort(int* p, int cap)
{
	m_ulCount = 0;
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

	m_ulCount = 0;
	memset(pBubble, 0, sizeof(pBubble[0]) * m_iCap);
	memcpy(pBubble, m_ipArray, sizeof(pBubble[0]) * m_iCap);

	for (i = 1; i < m_iCap; i++)
	{
		for (j = 0; j < m_iCap - i; j++)
		{
			m_ulCount++;

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
	printf("bubbleSort takes [%lu] Times when n = [%d] \n", m_ulCount, m_iCap);

	delete[]pBubble;
	return true;
}


bool MySort::fasteSort(UP_DOWN Up_Down)
{
	int i = 0, j = 0;
	DWORD count = 0;
	bool isSorted = true;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pFast = new int[m_iCap];
	if (NULL == pFast)
	{
		cout << "New Fail!" << endl;
		return false;
	}

	m_ulCount = 0;
	memset(pFast, 0, sizeof(pFast[0]) * m_iCap);
	memcpy(pFast, m_ipArray, sizeof(pFast[0]) * m_iCap);

	fastSortImp(pFast, m_iCap);

	showArray(pFast, m_iCap);
	printf("fasteSort takes [%lu] Times when n = [%d] \n", m_ulCount, m_iCap);


	delete[]pFast;
	return true;
}

bool MySort::fastSortImp(int* pFast, int iCap)
{
	// Exit recursion
	if (1 >= iCap)
	{
		return true;
	}

	// Implement
	int* pLeft, * pRight;
	int iMark = 0, iBaseIndex = 0, iCurrent = 0;
	int iBaseNum = pFast[iBaseIndex];

	for (iCurrent = 1; iCurrent < iCap; iCurrent++)
	{
		m_ulCount += 1;
		if (pFast[iCurrent] > iBaseNum)
		{
			iMark++;
			if (iCurrent != iMark)
			{
				SWAP(pFast[iCurrent], pFast[iMark]);
			}
		}
	}

	if (iBaseIndex != iMark)
	{
		SWAP(pFast[iBaseIndex], pFast[iMark]);
	}

	pLeft = pFast;
	pRight = (int*)&pFast[iMark + 1];
	
	// Recursion
	fastSortImp(pLeft, iMark);
	fastSortImp(pRight, iCurrent - iMark - 1);

	return true;
}

bool MySort::binTreeSort(UP_DOWN Up_Down)
{
	DWORD count = 0;
	BIG_LITTLE bigLittle = Up_Down == UP ? BIG : LITTLE;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pBinTree = new int[m_iCap];
	if (NULL == pBinTree)
	{
		cout << "New Fail!" << endl;
		return false;
	}

	m_ulCount = 0;
	memset(pBinTree, 0, sizeof(pBinTree[0]) * m_iCap);
	memcpy(pBinTree, m_ipArray, sizeof(pBinTree[0]) * m_iCap);

	BinTree B(pBinTree, m_iCap);
	for (int i = m_iCap - 1; i > 1; i--)
	{
		B.buildHeap(bigLittle);
		SWAP(B.m_piBinTree[0], B.m_piBinTree[i]);
		B.m_iCap--;
		m_ulCount += B.m_iCount;
	}

	memcpy(pBinTree, B.m_piBinTree, sizeof(pBinTree[0]) * m_iCap);

	showArray(pBinTree, m_iCap);
	printf("BinTree Sort takes [%lu] Times when n = [%d] \n", m_ulCount, m_iCap);


	delete[]pBinTree;
	return true;
}