#ifndef __MY_SEARCH_H__
#define __MY_SEARCH_H__

#include <iostream>
#include <string>

using namespace std;

class MySearch
{
public:
	MySearch(const int *pList, int iCap);
	~MySearch();

	bool listPrint(int* pList, int iLen);

	bool sequenceSearch(int target, int &index);

public:
	int *m_piSearchList;
	int m_iSearchCap;
};



#endif // !__MY_SEARCH_H__
