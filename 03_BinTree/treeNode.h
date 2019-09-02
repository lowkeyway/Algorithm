#ifndef  __TREE_NODE_H__
#define	__TREE_NODE_H__

#include <iostream>
#include <string>

using namespace std;

template <typename T>
class treeNode
{
public:
	treeNode(int index, T &Data);
	~treeNode();

	treeNode<T>* nodeSearch(int index);
	bool nodeDel();
	void preorderPrint();
	void inorderPrint();
	void postorderPrint();
	void nodePrint();

public:
	int m_iIndex;
	T* m_pData;
	treeNode<T>* m_pPar;
	treeNode<T>* m_pLChild;
	treeNode<T>* m_pRChild;
};

template <typename T>
treeNode<T>::treeNode(int index, T &Data)
{
	if (0 > index)
		return;
	m_pData = new T();
	if (NULL == m_pData)
		return;

	m_iIndex = index;
	*m_pData = Data;
	m_pPar = NULL;
	m_pLChild = NULL;
	m_pRChild = NULL;
}

template <typename T>
treeNode<T>::~treeNode()
{
	cout << "~treeNode" << endl;
	if (NULL != m_pData)
	{
		delete m_pData;
		m_pData = NULL;
	}

	m_pPar = NULL;
	m_pLChild = NULL;
	m_pRChild = NULL;
}

template <typename T>
treeNode<T>* treeNode<T>::nodeSearch(int index)
{
	treeNode<T>* temp;

	if (index < 0)
	{
		return NULL;
	}

	if (this->m_iIndex == index)
	{
		return this;
	}

	if (NULL != this->m_pLChild)
	{
		temp = this->m_pLChild->nodeSearch(index);
		if (NULL != temp)
			return temp;
	}

	if (NULL != this->m_pRChild)
	{
		temp = this->m_pRChild->nodeSearch(index);
		if (NULL != temp)
			return temp;
	}

	return NULL;
}

template <typename T>
bool treeNode<T>::nodeDel()
{
	if (NULL != this->m_pLChild)
	{
		this->m_pLChild->nodeDel();
	}

	if (NULL != this->m_pRChild)
	{
		this->m_pRChild->nodeDel();
	}

	if (NULL != this->m_pPar)
	{
		if (this->m_pPar->m_pLChild == this)
		{
			this->m_pPar->m_pLChild = NULL;
		}
		if (this->m_pPar->m_pRChild == this)
		{
			this->m_pPar->m_pRChild = NULL;
		}
	}

	//delete this;

	return true;
}

template <typename T>
void treeNode<T>::nodePrint()
{
	if (NULL == m_pData)
	{
		cout << "Node is NULL" << endl;
	}

	cout << m_iIndex << ": " << *m_pData << endl;;
}

// root->left->right
template <typename T>
void treeNode<T>::preorderPrint()
{
	//cout << this << endl;
	this->nodePrint();

	if (NULL != this->m_pLChild)
	{
		this->m_pLChild->preorderPrint();
	}

	if (NULL != this->m_pRChild)
	{
		this->m_pRChild->preorderPrint();
	}

	return;
}

// left->root->right
template <typename T>
void treeNode<T>::inorderPrint()
{
	if (NULL != this->m_pLChild)
	{
		this->m_pLChild->inorderPrint();
	}

	this->nodePrint();

	if (NULL != this->m_pRChild)
	{
		this->m_pRChild->inorderPrint();
	}

	return;
}

// left->root->right
template <typename T>
void treeNode<T>::postorderPrint()
{
	if (NULL != this->m_pLChild)
	{
		this->m_pLChild->postorderPrint();
	}

	if (NULL != this->m_pRChild)
	{
		this->m_pRChild->postorderPrint();
	}

	this->nodePrint();

	return;
}

#endif // ! __TREE_NODE_H__
