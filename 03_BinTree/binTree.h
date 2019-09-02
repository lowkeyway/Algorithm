#ifndef __BIN_TREE_H__
#define __BIN_TREE_H__

#include <iostream>
#include <string>
#include "treeNode.h"

using namespace std;

typedef enum {
	LEFT = 0,
	RIGHT,
} eLR;

template <typename T>
class binTree
{
public:
	binTree(treeNode<T>* pT = NULL);
	~binTree();

	treeNode<T>* treeSearch(int index);
	bool treeAdd(int index, eLR loc, treeNode<T>* pNode);
	bool treeDel(int index);
	void preorderPrint();// root->left->right
	void inorderPrint();//left->root->right
	void postorderPrint();//left->right->root

private:
	treeNode<T> * m_pRoot;
};

template <typename T>
binTree<T>::binTree(treeNode<T>* pT)
{
	m_pRoot = pT;
	if (NULL != m_pRoot)
	{
		m_pRoot->m_pPar = NULL;
		m_pRoot->m_pLChild = NULL;
		m_pRoot->m_pRChild = NULL;
	}
}

template <typename T>
binTree<T>::~binTree()
{
	if (NULL != m_pRoot)
	{
		m_pRoot->nodeDel();
	}
}

template <typename T>
treeNode<T>* binTree<T>::treeSearch(int index)
{
	//treeNode<T> * temp = m_pRoot;
	if (0 > index)
	{
		return NULL;
	}

	return m_pRoot->nodeSearch(index);
}

template <typename T>
bool binTree<T>::treeAdd(int index, eLR loc, treeNode<T>* pNode)
{
	treeNode<T>* temp;
	if (0 > index)
	{
		cout << "treeAdd fail, index should be a postive number!" << endl;
		return false;
	}

	temp = treeSearch(index);
	if (NULL == temp)
	{
		cout << "treeAdd fail, can not search this index node!" << endl;
		return false;
	}

	if (LEFT == loc)
	{
		if (NULL != temp->m_pLChild)
		{
			cout << "The tree node you want to insert is not NULL" << endl;
			return false;
		}

		temp->m_pLChild = pNode;
		pNode->m_pPar = temp;
		
		return true;
	}

	if (RIGHT == loc)
	{
		if (NULL != temp->m_pRChild)
		{
			cout << "The tree node you want to insert is not NULL" << endl;
			return false;
		}

		temp->m_pRChild = pNode;
		pNode->m_pPar = temp;

		return true;
	}
errorOut:
	return false;
}

template <typename T>
bool binTree<T>::treeDel(int index)
{
	treeNode<T>* temp;
	if (0 > index)
	{
		return false;
	}

	temp = treeSearch(index);
	if (NULL == temp)
	{
		return false;
	}

	return temp->nodeDel();
}

// root->left->right
template <typename T>
void binTree<T>::preorderPrint()
{
	if(NULL != m_pRoot)	
		m_pRoot->preorderPrint();
}

// left->root->right
template <typename T>
void binTree<T>::inorderPrint()
{
	if (NULL != m_pRoot)
		m_pRoot->inorderPrint();
}

// left->root->right
template <typename T>
void binTree<T>::postorderPrint()
{
	if (NULL != m_pRoot)
		m_pRoot->postorderPrint();
}

#endif // !__BIN_TREE_H__
