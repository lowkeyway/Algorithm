#include <iostream>
#include <string>

#include "MyBinTree.h"

#define MAX(X, Y)	((X)>(Y)?(X):(Y))

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
			m_pHead->height = 1;
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
		pBinTreeSearchInsert->height = 1;

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

int BinTree::getHeight(BinTreeNode *pTree)
{
	if (nullptr == pTree)
	{
		return 0;
	}

	return pTree->height;
}


bool BinTree::creatAvlTree(int* pData, int iLen)
{
	int i = -1;
	if (nullptr == pData || iLen <= 0)
	{
		cout << "Please init arrayList frist!" << endl;
		return false;
	}

	BinTreeNode *pBinTreeSearchInsert;
	for (int i = 0; i < iLen; i++)
	{
		if (0 == i)
		{
			m_pHead->node.data = pData[i];
			m_pHead->node.index = i;
			m_pHead->pLeft = nullptr;
			m_pHead->pRight = nullptr;
			m_pHead->height = 1;
			continue;
		}

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
		pBinTreeSearchInsert->height = 1;

		insertAvlTree(m_pHead, pBinTreeSearchInsert);
	}

	return true;
}



bool BinTree::insertAvlTree(BinTreeNode *&pParent, BinTreeNode *pInsert)
{
	if (pInsert->node.data >= pParent->node.data)
	{
		if (nullptr == pParent->pRight)
		{
			pParent->pRight = pInsert;
		}
		else
		{
			insertAvlTree(pParent->pRight, pInsert);
			if (getHeight(pParent->pRight) - getHeight(pParent->pLeft) == 2)
			{
				//cout << "Insert Data: " << pInsert->node.data << endl;
				//cout << pParent->node.data << "\t L: " << getHeight(pParent->pLeft) << ", R: " << getHeight(pParent->pRight) << endl;
				if (pInsert->node.data >= pParent->pRight->node.data)
				{
					pParent = RRRotation(pParent);
				}
				else
				{
					pParent = RLRotation(pParent);
				}
			}

		}
	}
	else if (pInsert->node.data < pParent->node.data)
	{
		if (nullptr == pParent->pLeft)
		{
			pParent->pLeft = pInsert;
		}
		else
		{
			insertAvlTree(pParent->pLeft, pInsert);
			if (getHeight(pParent->pLeft) - getHeight(pParent->pRight) == 2)
			{
				//cout << "Insert Data: " << pInsert->node.data << endl;
				//cout << pParent->node.data << "\t L: " << getHeight(pParent->pLeft) << ", R: " << getHeight(pParent->pRight) << endl;
				if (pInsert->node.data < pParent->pLeft->node.data)
				{
					pParent = LLRotation(pParent);
				}
				else
				{
					pParent = LRRotation(pParent);
				}
			}

		}
	}

	pParent->height = MAX(getHeight(pParent->pLeft), getHeight(pParent->pRight)) + 1;

	return true;
}


BinTreeNode * BinTree::LLRotation(BinTreeNode *pN0)
{
	BinTreeNode *pNL, *pNR;

	pNL = pN0->pLeft;
	pNR = pN0->pRight;

	pN0->pLeft = pNL->pRight;
	pNL->pRight = pN0;

	pN0->height = MAX(getHeight(pN0->pLeft), getHeight(pN0->pRight)) + 1;
	pNL->height = MAX(getHeight(pNL->pLeft), getHeight(pNL->pRight)) + 1;

	return pNL;
}

BinTreeNode * BinTree::RRRotation(BinTreeNode *pN0)
{
	BinTreeNode *pNL, *pNR;

	pNL = pN0->pLeft;
	pNR = pN0->pRight;

	pN0->pRight = pNR->pLeft;
	pNR->pLeft = pN0;

	pN0->height = MAX(getHeight(pN0->pLeft), getHeight(pN0->pRight)) + 1;
	pNR->height = MAX(getHeight(pNR->pLeft), getHeight(pNR->pRight)) + 1;

	return pNR;
}

BinTreeNode * BinTree::LRRotation(BinTreeNode *pN0)
{
	BinTreeNode *pNL, *pNR;

	pNL = pN0->pLeft;
	pNR = pN0->pRight;

	pNL = RRRotation(pNL);
	return LLRotation(pN0);
}

BinTreeNode * BinTree::RLRotation(BinTreeNode *pN0)
{
	BinTreeNode *pNL, *pNR;

	pNL = pN0->pLeft;
	pNR = pN0->pRight;

	pNR = LLRotation(pNR);
	return RRRotation(pN0);
}