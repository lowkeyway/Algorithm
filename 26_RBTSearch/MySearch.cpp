#include "MySearch.h"
#include <vector>

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

bool MySearch::listPrint(BinTreeNode* pListNode)
{
	if (nullptr == pListNode)
	{
		return false;
	}

	if (nullptr != pListNode->pLeft)
	{
		listPrint(pListNode->pLeft);
	}
	
	cout << "[" << pListNode->node.index << "]" << pListNode->node.data << " ";

	if (nullptr != pListNode->pRight)
	{
		listPrint(pListNode->pRight);
	}
	return true;
}


bool MySearch::sequenceSearch(int target, int &index)
{
	if (nullptr == m_piSearchList || m_iSearchCap <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	int *pSequeSearch = new int[m_iSearchCap];
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

	SearchNode *pBinarySearchNode = new SearchNode[m_iSearchCap];
	for (int i = 0; i < m_iSearchCap; i++)
	{
		pBinarySearchNode[i].data = m_piSearchList[i];
		pBinarySearchNode[i].index = i;
	}
	listPrint(pBinarySearchNode, m_iSearchCap);

	qsort(pBinarySearchNode, m_iSearchCap, sizeof(pBinarySearchNode[0]), comp);

	if (target > pBinarySearchNode[m_iSearchCap - 1].data || target < pBinarySearchNode[0].data)
	{
		cout << "Out of array" << endl;
		index = -1;
		return false;
	}

	int low = 0, high = m_iSearchCap, middle = 0;

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




bool MySearch::insertSearch(int target, int &index)
{
	if (nullptr == m_piSearchList || m_iSearchCap <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	SearchNode *pInsertSearchNode = new SearchNode[m_iSearchCap];
	for (int i = 0; i < m_iSearchCap; i++)
	{
		pInsertSearchNode[i].data = m_piSearchList[i];
		pInsertSearchNode[i].index = i;
	}
	listPrint(pInsertSearchNode, m_iSearchCap);

	qsort(pInsertSearchNode, m_iSearchCap, sizeof(pInsertSearchNode[0]), comp);

	if (target > pInsertSearchNode[m_iSearchCap - 1].data || target < pInsertSearchNode[0].data)
	{
		cout << "Out of array" << endl;
		index = -1;
		return false;
	}

	int low = 0, high = m_iSearchCap, middle = 0;

	while (low <= high)
	{
		middle = low + (target - pInsertSearchNode[low].data) * (high - low) / \
			(pInsertSearchNode[high].data - pInsertSearchNode[low].data);
		if (target == pInsertSearchNode[middle].data)
		{
			index = pInsertSearchNode[middle].index;
			break;
		}

		if (target > pInsertSearchNode[middle].data)
		{
			low = middle + 1;
			continue;
		}

		if (target < pInsertSearchNode[middle].data)
		{
			high = middle - 1;
			continue;
		}
	}
	listPrint(pInsertSearchNode, m_iSearchCap);

	delete[]pInsertSearchNode;
	return true;
}


bool MySearch::fibonacciSearch(int target, int &index)
{
	int i = -1;
	if (nullptr == m_piSearchList || m_iSearchCap <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	SearchNode *pFibonacciSearchNode = new SearchNode[m_iSearchCap];
	if (nullptr == pFibonacciSearchNode)
	{
		cout << "New pFibonacciSearchNode fail!" << endl;
		return false;
	}

	for (i = 0; i < m_iSearchCap; i++)
	{
		pFibonacciSearchNode[i].data = m_piSearchList[i];
		pFibonacciSearchNode[i].index = i;
	}
	listPrint(pFibonacciSearchNode, m_iSearchCap);

	qsort(pFibonacciSearchNode, m_iSearchCap, sizeof(pFibonacciSearchNode[0]), comp);

	if (target > pFibonacciSearchNode[m_iSearchCap - 1].data || target < pFibonacciSearchNode[0].data)
	{
		cout << "Out of array" << endl;
		index = -1;
		return false;
	}

	// Step1: Find the fib num which equal or bigger than target
	vector<int> vFib;
	int vFibSize = -1;
	int fibT0 = 0, fibT1 = 1, fibTn = -1;
	vFib.push_back(fibT0);
	vFib.push_back(fibT1);
	for (i = 1; vFib[i] < m_iSearchCap; i++)
	{
		fibTn = vFib[i] + vFib[i - 1];
		vFib.push_back(fibTn);
	}
	vFibSize = i;

	// Step2: Extend size of searchList to vfib[i]
	SearchNode *pFibonacciSearchNodeEx = new SearchNode[fibTn];
	if(nullptr == pFibonacciSearchNodeEx)
	{
		cout << "New pFibonacciSearchNodeEx Fail!" << endl;
		return false;
	}
	memset(pFibonacciSearchNodeEx, 0, sizeof(SearchNode)*fibTn);
	memcpy(pFibonacciSearchNodeEx, pFibonacciSearchNode, sizeof(SearchNode)*m_iSearchCap);
	for (i = m_iSearchCap; i < fibTn; i++)
	{
		pFibonacciSearchNodeEx[i] = pFibonacciSearchNode[m_iSearchCap - 1];
	}

	// Step3: Fibonacc Division
	int low = 0, high = vFibSize, middle = 0;

	while (true)
	{
		middle = low + vFib[vFibSize - 1] - 1;
		if (target == pFibonacciSearchNodeEx[middle].data)
		{
			index = pFibonacciSearchNodeEx[middle].index;
			break;
		}

		if (target > pFibonacciSearchNodeEx[middle].data)
		{
			low = middle + 1;
			vFibSize--;
			continue;
		}

		if (target < pFibonacciSearchNodeEx[middle].data)
		{
			vFibSize -= 2;
			continue;
		}
	}
	listPrint(pFibonacciSearchNodeEx, fibTn);

	delete[]pFibonacciSearchNode;
	delete[]pFibonacciSearchNodeEx;
	return true;
}

bool MySearch::binTreeSearch(int target, int &index)
{
	int i = -1;
	if (nullptr == m_piSearchList || m_iSearchCap <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	listPrint(m_piSearchList, m_iSearchCap);

	// Creat binSearchTree 
	BinTree B;
	B.creatSearchTree(m_piSearchList, m_iSearchCap);

	listPrint(B.m_pHead);
	cout << endl;

	// Search target
	B.searchSearchTree(target, index);

	return true;
}


bool MySearch::AVLTreeSearch(int target, int &index)
{
	int i = -1;
	if (nullptr == m_piSearchList || m_iSearchCap <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	listPrint(m_piSearchList, m_iSearchCap);

	// Creat binSearchTree 
	BinTree A;
	A.creatAvlTree(m_piSearchList, m_iSearchCap);

	listPrint(A.m_pHead);
	cout << endl;

	cout << "Lengh of AVL Tree: " << A.m_pHead->height << endl;
	cout << "Head of AVL Tree: " << A.m_pHead->node.data << endl;
	// Search target
	A.searchSearchTree(target, index);

	return true;
}

bool MySearch::RBTreeSearch(int target, int &index)
{
	int i = -1;
	if (nullptr == m_piSearchList || m_iSearchCap <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	listPrint(m_piSearchList, m_iSearchCap);

	// Creat binSearchTree 
	BinTree R;
	R.creatRBTree(m_piSearchList, m_iSearchCap);

	listPrint(R.m_pHead);
	cout << endl;

	cout << "Color of RB Head: " << R.m_pHead->Color << endl;
	cout << "Head of RB Tree: " << R.m_pHead->node.data << endl;
	// Search target
	R.searchSearchTree(target, index);

	return true;
}