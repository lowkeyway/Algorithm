#ifndef __LIST_NODE_H__
#define __LIST_NODE_H__

#include <iostream>

using namespace std;

template <typename T>
class listNode
{
public:
	listNode(T* pData);
	~listNode();

public:
	T* m_pData;
	listNode* m_pPre;
	listNode* m_pNext;
};

template <typename T>
listNode<T>::listNode(T* pData)
{
	m_pData = pData;
	m_pNext = this;
	m_pPre = this;
}
template <typename T>
listNode<T>::~listNode()
{
	m_pData = NULL;
	m_pNext = NULL;
	m_pPre = NULL;
}

#endif // !__LIST_NODE_H__
