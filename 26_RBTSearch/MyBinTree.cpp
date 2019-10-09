#include <iostream>
#include <string>

#include "MyBinTree.h"

#define MAX(X, Y)	((X)>(Y)?(X):(Y))
#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color == RED)
#define rb_is_black(r)  ((r)->color == BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r, p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r, c)  do { (r)->color = (c); } while (0)

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
			m_pHead->pParent = nullptr;
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
		pBinTreeSearchInsert->pParent = nullptr;
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
			pInsert->pParent = pParent;
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
			pInsert->pParent = pParent;
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

// Turn Right Rotation
BinTreeNode * BinTree::LLRotation(BinTreeNode *pN0)
{
	BinTreeNode *pNL, *pNR, *pNP;

	pNL = pN0->pLeft;
	pNR = pN0->pRight;
	pNP = pN0->pParent;

	// Step1#
	pN0->pLeft = pNL->pRight;
	if (nullptr != pNL->pRight)
	{
		pNL->pRight->pParent = pN0;
	}

	// Step2#
	pNL->pParent = pN0->pParent;

	if (nullptr == pN0->pParent)
	{
		m_pHead = pNL;
	}
	else if (pN0 == pN0->pParent->pLeft)
	{
		pN0->pParent->pLeft = pNL;
	}
	else
	{
		pN0->pParent->pRight = pNL;
	}

	// Step 3#
	pNL->pRight = pN0;
	pN0->pParent = pNL;

	pN0->height = MAX(getHeight(pN0->pLeft), getHeight(pN0->pRight)) + 1;
	pNL->height = MAX(getHeight(pNL->pLeft), getHeight(pNL->pRight)) + 1;

	return pNL;
}

// Turn Left Rotation
BinTreeNode * BinTree::RRRotation(BinTreeNode *pN0)
{
	BinTreeNode *pNL, *pNR, *pNP;

	pNL = pN0->pLeft;
	pNR = pN0->pRight;
	pNP = pN0->pParent;

	// Step1#
	pN0->pRight = pNR->pLeft;
	if (nullptr != pNR->pLeft)
	{
		pNR->pLeft->pParent = pN0;
	}

	// Step2#
	pNR->pParent = pN0->pParent;

	if (nullptr == pN0->pParent)
	{
		m_pHead = pNR;
	}
	else if(pN0 == pN0->pParent->pLeft)
	{
		pN0->pParent->pLeft = pNR;
	}
	else
	{
		pN0->pParent->pRight = pNR;
	}

	// Step 3#
	pNR->pLeft = pN0;
	pN0->pParent = pNR;


	pN0->height = MAX(getHeight(pN0->pLeft), getHeight(pN0->pRight)) + 1;
	pNR->height = MAX(getHeight(pNR->pLeft), getHeight(pNR->pRight)) + 1;

	return pNR;
}

BinTreeNode * BinTree::LRRotation(BinTreeNode *pN0)
{
	BinTreeNode *pNL, *pNR, *pNP;

	pNL = pN0->pLeft;
	pNR = pN0->pRight;
	pNP = pN0->pParent;

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

bool BinTree::creatRBTree(int* pData, int iLen)
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
			m_pHead->pParent = nullptr;
			m_pHead->Color = BLACK;
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
		pBinTreeSearchInsert->pParent = nullptr;
		pBinTreeSearchInsert->Color = RED;

		insertRBTree(m_pHead, pBinTreeSearchInsert);
	}

	return true;
}

bool BinTree::insertRBTree(BinTreeNode *&pParent, BinTreeNode *pInsert)
{
	BinTreeNode *pNP = pParent, *pN0 = nullptr;
	RBTDirection Dir = LEFT;

	while (pNP != nullptr)
	{
		pN0 = pNP;
		if (pInsert->node.data >= pParent->node.data)
		{
			pNP = pNP->pRight;
		}
		else
		{
			pNP = pNP->pLeft;
		}
	}

	pInsert->pParent = pN0;
	if (pInsert->node.data >= pN0->node.data)
	{
		pN0->pRight = pInsert;
	}
	else
	{
		pN0->pLeft = pInsert;
	}


	BinTreeNode *parent, *gparent, *node, *uncle, *temp;

	node = pInsert;
	parent = node->pParent;


	// If parent is black, do nothing.
	if (BLACK == parent->Color)
	{
		return true;
	}

	// parent is RED
	while (nullptr != node->pParent && node->pParent->Color == RED)
	{
		parent = node->pParent;
		gparent = parent->pParent;

		if (parent == gparent->pLeft)
		{
			uncle = gparent->pRight;
			// Case1#
			if (uncle && (RED == uncle->Color))
			{
				parent->Color = BLACK;
				uncle->Color = BLACK;
				gparent->Color = RED;
				node = gparent;
				continue;
			}

			// Case 2#
			if (node == parent->pRight)
			{
				RRRotation(parent);
				temp = parent;
				parent = node;
				node = temp;
			}

			// Case3#
			parent->Color = BLACK;
			gparent->Color = RED;
			LLRotation(gparent);
		}
		else
		{
			uncle = gparent->pLeft;
			// Case1#
			if (uncle && (RED == uncle->Color))
			{
				parent->Color = BLACK;
				uncle->Color = BLACK;
				gparent->Color = RED;
				node = gparent;
				continue;
			}

			// Case 2#
			if (node == parent->pLeft)
			{
				LLRotation(parent);
				temp = parent;
				parent = node;
				node = temp;
			}

			// Case 3#
			parent->Color = BLACK;
			gparent->Color = RED;
			RRRotation(gparent);
		}
	}

	m_pHead->Color = BLACK;

	return true;
}
