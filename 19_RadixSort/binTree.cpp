#include "binTree.h"


BinTree::BinTree(int* p, int cap)
{
	if (cap <= 0)
	{
		cout << "Please input correct number!" << endl;
		return;
	}

	m_piBinTree = new int[cap];
	if (NULL == m_piBinTree)
	{
		cout << "BinTree NEW fail!" << endl;
		return;
	}

	memset(m_piBinTree, 0, sizeof(int)*cap);
	memcpy(m_piBinTree, p, sizeof(int)*cap);

	m_iCap = cap;
	m_iCount = 0;
}

BinTree::~BinTree()
{
	delete[]m_piBinTree;
}

bool BinTree::showBinTree()
{
	if (m_iCap <= 0)
	{
		cout << "Please init BinTree frist!" << endl;
		return false;
	}

	cout << "BinTree: " << endl;
	for (int i = 0; i < m_iCap; i++)
	{
		cout << m_piBinTree[i] << " ";
	}
	cout << endl;

	return true;
}

bool BinTree::buildHeap(BIG_LITTLE bl)
{
	int childIndex, parentIndex;
	if (m_iCap <= 0)
	{
		cout << "Please init BinTree frist!" << endl;
		return false;
	}
	m_iCount = 0;
	for (int i = (m_iCap - 2) / 2; i >= 0; i--)
	{
		if (BIG == bl)
		{
			parentIndex = i;
			childIndex = 2 * parentIndex + 1;
			while (childIndex < m_iCap)
			{
				if (childIndex + 1 < m_iCap && m_piBinTree[childIndex] < m_piBinTree[childIndex + 1])
				{
					childIndex++;
				}

				if (m_piBinTree[parentIndex] < m_piBinTree[childIndex])
				{
					SWAP(m_piBinTree[parentIndex], m_piBinTree[childIndex]);
				}

				parentIndex = childIndex;
				childIndex = 2 * childIndex + 1;
				m_iCount++;
			}
		}
		else if (LITTLE == bl)
		{
			parentIndex = i;
			childIndex = 2 * parentIndex + 1;
			while (childIndex < m_iCap)
			{
				if (childIndex + 1 < m_iCap && m_piBinTree[childIndex] > m_piBinTree[childIndex + 1])
				{
					childIndex++;
				}

				if (m_piBinTree[parentIndex] > m_piBinTree[childIndex])
				{
					SWAP(m_piBinTree[parentIndex], m_piBinTree[childIndex]);
				}

				parentIndex = childIndex;
				childIndex = 2 * childIndex + 1;
				m_iCount++;
			}
		}
		else
		{
			cout << "SomeThing Wrong!" << endl;
			return false;
		}
	}

	return true;
}