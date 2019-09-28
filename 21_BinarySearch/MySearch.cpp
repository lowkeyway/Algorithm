#include "MySearch.h"

MySearch::MySearch(const int *pList, int iCap)
{
	if (nullptr == pList || iCap <= 0)
	{
		return;
	}

	m_iSearchCap = iCap;
	m_piSearchList = new int[iCap];
	if (nullptr == m_piSearchList)
	{
		return;
	}

	memset(m_piSearchList, 0, sizeof(int)*iCap);
	memcpy(m_piSearchList, pList, sizeof(int)*iCap);
}

MySearch::~MySearch()
{
	delete[]m_piSearchList;
}

bool MySearch::listPrint(int* pList, int iLen)
{
	if (nullptr == pList || iLen <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	for (int i = 0; i < iLen; i++)
	{
		cout << pList[i] << " ";
	}
	cout << endl;

	return true;
}

bool MySearch::listPrint(SearchNode* pListNode, int iLen)
{
	if (nullptr == pListNode || iLen <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	for (int i = 0; i < iLen; i++)
	{
		cout << "[" << pListNode[i].index << "]" << pListNode[i].data << " ";
	}
	cout << endl;

	return true;
}


bool MySearch::sequenceSearch(int target, int &index)
{
	if (nullptr == m_piSearchList || m_iSearchCap <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	int *pSequeSearch = new int[sizeof(int)*m_iSearchCap];
	memset(pSequeSearch, 0, sizeof(int)*m_iSearchCap);
	memcpy(pSequeSearch, m_piSearchList, sizeof(int)*m_iSearchCap);
	listPrint(pSequeSearch, m_iSearchCap);

	for (int i = 0; i < m_iSearchCap; i++)
	{
		if (target == m_piSearchList[i])
		{
			index = i;
			return true;
		}
	}

	delete []pSequeSearch;

	return false;
}

int comp(const void *a, const void *b)
{
	return (*(SearchNode *)a).data > (*(SearchNode *)b).data ? 1 : -1;
}

bool MySearch::binarySearch(int target, int &index)
{
	if (nullptr == m_piSearchList || m_iSearchCap <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	SearchNode *pBinarySearchNode = new SearchNode[sizeof(SearchNode)*m_iSearchCap];
	for (int i = 0; i < m_iSearchCap; i++)
	{
		pBinarySearchNode[i].data = m_piSearchList[i];
		pBinarySearchNode[i].index = i;
	}

	qsort(pBinarySearchNode, m_iSearchCap, sizeof(pBinarySearchNode[0]), comp);

	if (target > pBinarySearchNode[m_iSearchCap - 1].data || target < pBinarySearchNode[0].data)
	{
		cout << "Out of array" << endl;
		index = -1;
		return false;
	}

	int low = 0, high = m_iSearchCap, middle = 0;
	listPrint(pBinarySearchNode, m_iSearchCap);

	while (low <= high)
	{
		middle = low + (high - low) / 2;
		if (target == pBinarySearchNode[middle].data)
		{
			index = pBinarySearchNode[middle].index;
			break;
		}

		if (target > pBinarySearchNode[middle].data)
		{
			low = middle + 1;
			continue;
		}

		if (target < pBinarySearchNode[middle].data)
		{
			high = middle - 1;
			continue;
		}
	}
	listPrint(pBinarySearchNode, m_iSearchCap);

	delete[]pBinarySearchNode;
	return true;
}