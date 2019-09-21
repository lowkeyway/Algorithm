#ifndef __MY_SORT_H__
#define __MY_SORT_H__

#include <iostream>
#include <string>
#include "binTree.h"


typedef enum UpDown
{
	UP = 0,
	DOWN = 1
}UP_DOWN;

class MySort
{
public:
	MySort(int* p, int cap);
	~MySort();

	bool showArray(int* p, int len);
	bool fastSortImp(int *pFast, int iCap);
	bool mergeSortImp(int *pMerge, int iCap, int *pTemp, UP_DOWN Up_Down);

	bool bubbleSort(UP_DOWN Up_Down);
	bool fasteSort(UP_DOWN Up_Down);
	bool binTreeSort(UP_DOWN Up_Down);
	bool countSort(UP_DOWN Up_Down);
	bool bucketSort(UP_DOWN Up_Down);
	bool insertSort(UP_DOWN Up_Down);
	bool shellSort(UP_DOWN Up_Down);
	bool selectionSort(UP_DOWN Up_Down);
	bool mergeSort(UP_DOWN Up_Down);

public:
	int* m_ipArray;
	int m_iCap = 0;
	unsigned long m_ulCount;
};

#endif // !__MY_SORT_H__
