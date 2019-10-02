#include <iostream>
#include <string>

#include "MyBinTree.h"


using namespace std;

BinTree::BinTree()
{
	m_pHead = new BinTreeNode;
	if (nullptr == m_pHead)
	{
		cout << "New pBinTreeSearchHead Fail!" << endl;
	}
}

BinTree::~BinTree()
{
	deleteNode(m_pHead);
}

bool BinTree::deleteNode(BinTreeNode *pNode)
{
	if (nullptr == pNode)
	{
		cout << "Cannot free a NULL node!" << endl;
		return false;
	}
	if (nullptr != pNode->pLeft)
	{
		deleteNode(pNode->pLeft);
	}

	if (nullptr != pNode->pRight)
	{
		deleteNode(pNode->pRight);
	}

	delete pNode;

	return true;
}



bool BinTree::creatSearchTree(int* pData, int iLen)
{
	int i = -1;
	if (nullptr == pData || iLen <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	BinTreeNode *pBinTreeSearchInsert, *pBinTreeSearchTemp;
	for (i = 0; i < iLen; i++)
	{
		if (0 == i)
		{
			m_pHead->node.data = pData[i];
			m_pHead->node.index = i;
			m_pHead->pLeft = nullptr;
			m_pHead->pRight = nullptr;
			continue;
		}
		pBinTreeSearchTemp = m_pHead;
		pBinTreeSearchInsert = new BinTreeNode;
		if (nullptr == pBinTreeSearchInsert)
		{
			cout << "New pBinTreeSearchInsert Fail!" << endl;
			return false;
		}

		pBinTreeSearchInsert->node.data = pData[i];
		pBinTreeSearchInsert->node.index = i;
		pBinTreeSearchInsert->pLeft = nullptr;
		pBinTreeSearchInsert->pRight = nullptr;

		while (true)
		{
			if (pBinTreeSearchInsert->node.data <= pBinTreeSearchTemp->node.data)
			{
				if (nullptr != pBinTreeSearchTemp->pLeft)
				{
					pBinTreeSearchTemp = pBinTreeSearchTemp->pLeft;
					continue;
				}

				pBinTreeSearchTemp->pLeft = pBinTreeSearchInsert;
				break;
			}

			if (pBinTreeSearchInsert->node.data > pBinTreeSearchTemp->node.data)
			{
				if (nullptr != pBinTreeSearchTemp->pRight)
				{
					pBinTreeSearchTemp = pBinTreeSearchTemp->pRight;
					continue;
				}

				pBinTreeSearchTemp->pRight = pBinTreeSearchInsert;
				break;
			}
		}

	}
}

bool BinTree::searchSearchTree(int target, int &index)
{
	BinTreeNode *pBinTreeSearchTemp = m_pHead;
	while (nullptr != pBinTreeSearchTemp)
	{
		if (target == pBinTreeSearchTemp->node.data)
		{
			index = pBinTreeSearchTemp->node.index;
			return true;
		}

		if (target > pBinTreeSearchTemp->node.data)
		{
			if (nullptr == pBinTreeSearchTemp->pRight)
			{
				cout << "Cannot find target!" << endl;
				return false;
			}

			pBinTreeSearchTemp = pBinTreeSearchTemp->pRight;
		}

		if (target < pBinTreeSearchTemp->node.data)
		{
			if (nullptr == pBinTreeSearchTemp->pLeft)
			{
				cout << "Cannot find target!" << endl;
				return false;
			}

			pBinTreeSearchTemp = pBinTreeSearchTemp->pLeft;
		}
	}

	return false;
}
