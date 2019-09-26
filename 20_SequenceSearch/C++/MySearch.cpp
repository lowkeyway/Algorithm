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

	return false;
}