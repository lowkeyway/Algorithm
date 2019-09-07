#ifndef __MY_SORT_H__
#define __MY_SORT_H__

#include <iostream>
#include <string>

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

	bool bubbleSort(UP_DOWN Up_Down);
	bool fasteSort(UP_DOWN Up_Down);
	bool fastSortImp(int *pFast, int iCap);

public:
	int* m_ipArray;
	int m_iCap = 0;
	unsigned long m_ulCount;
};

#endif // !__MY_SORT_H__
