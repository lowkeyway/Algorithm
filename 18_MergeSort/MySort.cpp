#include "MySort.h"
#include <Windows.h>
#include <vector>
#include <algorithm>

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
	if (nullptr == pBubble)
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
	bool isSorted = true;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pFast = new int[m_iCap];
	if (nullptr == pFast)
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
	BIG_LITTLE bigLittle = Up_Down == UP ? BIG : LITTLE;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pBinTree = new int[m_iCap];
	if (nullptr == pBinTree)
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


bool MySort::countSort(UP_DOWN Up_Down)
{
	DWORD count = 0;
	int max, min, temp = 0;
	int i = 0;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pCount = new int[m_iCap];
	if (nullptr == pCount)
	{
		cout << "New pCount Fail!" << endl;
		return false;
	}
	int* pSort = new int[m_iCap];
	if (nullptr == pSort)
	{
		cout << "New pSort Fail!" << endl;
		return false;
	}
	m_ulCount = 0;
	memset(pCount, 0, sizeof(pCount[0]) * m_iCap);
	memset(pSort, 0, sizeof(pSort[0]) * m_iCap);
	memcpy(pCount, m_ipArray, sizeof(pCount[0]) * m_iCap);
	memcpy(pSort, m_ipArray, sizeof(pSort[0]) * m_iCap);

	// Calculate the length of array we need to new
	max = pCount[0], min = pCount[0];
	for (i = 1; i < m_iCap; i++)
	{
		if (max < pCount[i])
		{
			max = pCount[i];
		}
		if (min > pCount[i])
		{
			min = pCount[i];
		}
		m_ulCount++;
	}

	// New Array
	int arrLen = max - min + 1;
	int *pArray = new int[arrLen];
	if (nullptr == pArray)
	{
		cout << "New pArray fail" << endl;
		return false;
	}
	memset(pArray, 0, sizeof(int)*(arrLen));

	// Loop element
	for (i = 0; i < m_iCap; i++)
	{
		pArray[pCount[i] - min]++;
		m_ulCount++;
	}

	for (i = 1; i < arrLen; i++)
	{
		pArray[i] = pArray[i] + pArray[i - 1];
		m_ulCount++;
	}

	// Remap element
	for (i = 0; i < m_iCap; i++)
	{
		if (UP == Up_Down)
		{
			pSort[pArray[pCount[i] - min] - 1] = pCount[i];
		}
		else if (DOWN == Up_Down)
		{
			pSort[m_iCap - 1 - (pArray[pCount[i] - min] - 1)] = pCount[i];
		}
		m_ulCount++;
	}

	showArray(pSort, m_iCap);
	printf("Count Sort takes [%lu] Times when n = [%d] \n", m_ulCount, m_iCap);


	delete[]pArray;
	delete[]pCount;
	delete[]pSort;
	return true;
}


bool MySort::bucketSort(UP_DOWN Up_Down)
{
	int min, max;
	int i = 0;
	int secArray, secBucket, numBucket;
	vector<vector<int>> vecBucket;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pBucket = new int[m_iCap];
	if (nullptr == pBucket)
	{
		cout << "New pBucket Fail!" << endl;
		return false;
	}

	m_ulCount = 0;
	memset(pBucket, 0, sizeof(pBucket[0]) * m_iCap);
	memcpy(pBucket, m_ipArray, sizeof(pBucket[0]) * m_iCap);

	// Find the max/min/lenArry
	min = pBucket[0], max = pBucket[0];
	for (i = 0; i < m_iCap; i++)
	{
		if (max < pBucket[i])
		{
			max = pBucket[i];
		}
		if (min > pBucket[i])
		{
			min = pBucket[i];
		}
		m_ulCount++;
	}
	secArray = max - min + 1;

	// The roles of bucket
	numBucket = m_iCap / 2;
	secBucket = (max - min) / (numBucket - 1);
	vecBucket.resize(numBucket);
	cout << "Sizeof vecBucket: " << vecBucket.size() << endl;
	// Put element to bucket
	for (i = 0; i < m_iCap; i++)
	{
		vecBucket[(pBucket[i] - min) / secBucket].push_back(pBucket[i]);
		m_ulCount++;
	}

	// Sort in the bucket
	for (vector<vector<int>>::iterator vit = vecBucket.begin(); vit != vecBucket.end(); vit++)
	{
		if (vit->empty())
			continue;
		
		sort(vit->begin(), vit->end());

		m_ulCount++;
	}

	// Remap bucket element to array
	int index = 0;
	for (vector<vector<int>>::iterator vit = vecBucket.begin(); vit != vecBucket.end(); vit++)
	{
		if (vit->empty())
		{
			continue;
		}

		for (vector<int>::iterator iit = vit->begin(); iit != vit->end(); iit++)
		{
			if (UP == Up_Down)
			{
				pBucket[index] = *iit;
			}
			else if (DOWN == Up_Down)
			{
				pBucket[m_iCap - index -1] = *iit;
			}
			else
			{
				cout << "Incorrect Input" << endl;
				return false;
			}
			index++;
			m_ulCount++;
		}

	}


	showArray(pBucket, m_iCap);
	printf("Bucket Sort takes [%lu] Times when n = [%d] \n", m_ulCount, m_iCap);

	delete[]pBucket;
	return true;
}

bool MySort::insertSort(UP_DOWN Up_Down)
{
	int i = 0, j = 0;
	int temp;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pInsert = new int[m_iCap];
	if (nullptr == pInsert)
	{
		cout << "New pInsert Fail!" << endl;
		return false;
	}

	m_ulCount = 0;
	memset(pInsert, 0, sizeof(pInsert[0]) * m_iCap);
	memcpy(pInsert, m_ipArray, sizeof(pInsert[0]) * m_iCap);

	for (i = 1; i < m_iCap; i++)
	{
		temp = pInsert[i];
		for (j = i; j > 0; j--)
		{
			m_ulCount++;
			if (((UP == Up_Down) && (pInsert[j - 1] > temp)) ||\
				((DOWN == Up_Down) && (pInsert[j - 1] < temp)))
			{
				pInsert[j] = pInsert[j - 1];
				continue;
			}
			break;
		}
		pInsert[j] = temp;
	}

	showArray(pInsert, m_iCap);
	printf("Insert Sort takes [%lu] Times when n = [%d] \n", m_ulCount, m_iCap);

	delete[]pInsert;
	return true;
}


bool MySort::shellSort(UP_DOWN Up_Down)
{
	int i = 0, j = 0, k = 0, step = 0;
	int temp;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pShell = new int[m_iCap];
	if (nullptr == pShell)
	{
		cout << "New pShell Fail!" << endl;
		return false;
	}

	m_ulCount = 0;
	memset(pShell, 0, sizeof(pShell[0]) * m_iCap);
	memcpy(pShell, m_ipArray, sizeof(pShell[0]) * m_iCap);

	step = m_iCap;	
	do{
		step /= 2;
		for (i = 0; i < step; i++)
		{
			for (j = i + step; j < m_iCap; j += step)
			{
				temp = pShell[j];
				for (k = j; k - step >= 0; k -= step)
				{
					m_ulCount++;
					if (((UP == Up_Down) && (pShell[k - step] > temp)) || \
						((DOWN == Up_Down) && (pShell[k - step] < temp)))
					{
						pShell[k] = pShell[k - step];
						continue;
					}
					break;
				}
				pShell[k] = temp;
			}
		}
	} while (step);

	showArray(pShell, m_iCap);
	printf("Shell takes [%lu] Times when n = [%d] \n", m_ulCount, m_iCap);

	delete[]pShell;
	return true;
}

bool MySort::selectionSort(UP_DOWN Up_Down)
{
	int i = 0, j = 0;
	int min, max, minIndex, maxIndex;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pSelect = new int[m_iCap];
	if (nullptr == pSelect)
	{
		cout << "New pShell Fail!" << endl;
		return false;
	}

	m_ulCount = 0;
	memset(pSelect, 0, sizeof(pSelect[0]) * m_iCap);
	memcpy(pSelect, m_ipArray, sizeof(pSelect[0]) * m_iCap);

	for (i = 0; i < m_iCap; i++)
	{
		min = pSelect[i], minIndex = i;
		max = pSelect[i], maxIndex = i;
		for (j = i + 1; j < m_iCap; j++)
		{
			m_ulCount++;
			if (UP == Up_Down && min > pSelect[j])
			{
				min = pSelect[j];
				minIndex = j;
			}
			else if (DOWN == Up_Down && max < pSelect[j])
			{
				max = pSelect[j];
				maxIndex = j;
			}
			else
			{
				continue;
			}
		}

		if (UP == Up_Down && minIndex != i)
		{
			SWAP(pSelect[i], pSelect[minIndex]);
		}
		else if (DOWN == Up_Down && maxIndex != i)
		{
			SWAP(pSelect[i], pSelect[maxIndex]);
		}
		else
		{
			continue;
		}
	}

	showArray(pSelect, m_iCap);
	printf("Selection sort takes [%lu] Times when n = [%d] \n", m_ulCount, m_iCap);

	delete[]pSelect;
	return true;
}

bool MySort::mergeSortImp(int *pMer, int iCap, int *pTemp, UP_DOWN Up_Down)
{
	if (1 >= iCap)
	{
		return true;
	}

	int *pLeft, *pRight;
	int lenLeft, lenRight, indLeft, indRight;

	pLeft = pMer, lenLeft = iCap / 2;
	pRight = pMer + lenLeft, lenRight = iCap - lenLeft;

	mergeSortImp(pLeft, lenLeft, pTemp, Up_Down);
	mergeSortImp(pRight, lenRight, pTemp, Up_Down);

	int i;
	for (i = 0; i < iCap; i++)
	{
		pTemp[i] = pMer[i];
	}

	indLeft = 0, indRight = lenLeft;
	for (i = 0; i < iCap; i++)
	{
		m_ulCount++;
		if (indLeft == lenLeft)
		{
			pMer[i] = pTemp[indRight++];
			continue;
		}

		if (indRight == iCap)
		{
			pMer[i] = pTemp[indLeft++];
			continue;
		}

		if(UP == Up_Down)
		{
			if (pTemp[indLeft] <= pTemp[indRight])
			{
				pMer[i] = pTemp[indLeft++];
			}
			else
			{
				pMer[i] = pTemp[indRight++];
			}
		}
		else if (DOWN == Up_Down)
		{
			if (pTemp[indLeft] > pTemp[indRight])
			{
				pMer[i] = pTemp[indLeft++];
			}
			else
			{
				pMer[i] = pTemp[indRight++];
			}
		}

	}

	return true;
}

bool MySort::mergeSort(UP_DOWN Up_Down)
{
	int i = 0, j = 0;
	int min, max, minIndex, maxIndex;

	if (m_iCap <= 0)
	{
		cout << "Please input array frist!" << endl;
		return false;
	}

	int* pMerge = new int[m_iCap];
	int* pTemp = new int[m_iCap];
	if (nullptr == pMerge || nullptr == pTemp)
	{
		cout << "New pMerge or pTemp Fail!" << endl;
		return false;
	}

	m_ulCount = 0;
	memset(pMerge, 0, sizeof(pMerge[0]) * m_iCap);
	memset(pTemp, 0, sizeof(pTemp[0]) * m_iCap);
	memcpy(pMerge, m_ipArray, sizeof(pMerge[0]) * m_iCap);


	mergeSortImp(pMerge, m_iCap, pTemp, Up_Down);

	showArray(pMerge, m_iCap);
	printf("Merge sort takes [%lu] Times when n = [%d] \n", m_ulCount, m_iCap);

	delete[]pMerge;
	delete[]pTemp;
	return true;
}