#ifndef __MY_SEARCH_H__
#define __MY_SEARCH_H__

#include <iostream>
#include <string>
#include"MyBinTree.h"

using namespace std;

typedef struct searchData {
	int data;
	int index;
}SearchNode;


class MySearch
{
public:
	MySearch(const int *pList, int iCap);
	~MySearch();

	bool listPrint(int* pList, int iLen);
	bool listPrint(SearchNode* pListNode, int iLen);
	bool listPrint(BinTreeNode* pListNode);

	bool sequenceSearch(int target, int &index);
	bool binarySearch(int target, int &index);
	bool insertSearch(int target, int &index);
	bool fibonacciSearch(int target, int &index);
	bool binTreeSearch(int target, int &index);

public:
	int *m_piSearchList;
	int m_iSearchCap;
};



#endif // !__MY_SEARCH_H__
