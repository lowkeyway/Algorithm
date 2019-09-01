#ifndef __MY_LIST_H__
#define __MY_LIST_H__

#include <iostream>
#include <string>
#include "listNode.h"

using namespace std;

template <typename T>
class myList
{
public:
	myList(int size);
	~myList();

	bool addTail(T &data);
	bool outHead(T &data);

	bool add(int index, T &data);
	bool out(int index, T &data);

	bool isEmpty();
	bool isFull();

	void listPrint();

private:
	listNode<T>* m_pHead;
	listNode<T>* m_pTail;

	int m_iListSize;
	int m_iListCapacity;
};

template <typename T>
myList<T>::myList(int size)
{
	m_pHead = NULL;
	m_pTail = NULL;

	m_iListCapacity = size;
	m_iListSize = 0;
}
template <typename T>
myList<T>::~myList()
{
	m_pHead = NULL;
	m_pTail = NULL;

	m_iListCapacity = 0;
	m_iListSize = 0;
}

template <typename T>
bool myList<T>::addTail(T &data)
{
	if (isFull())
	{
		cout << "The list is full, size is " << m_iListSize << endl;
		return -1;
	}

	listNode<T> *pNode = new listNode<T>(&data);

	if (isEmpty())
	{
		cout << "Frist input" << endl;
		m_pHead = pNode;
		m_pTail = pNode;
		m_iListSize++;
		return 0;
	}

	pNode->m_pNext = m_pHead;
	pNode->m_pPre = m_pTail;
	m_pTail->m_pNext = pNode;
	m_pHead->m_pPre = pNode;

	m_pTail = pNode;
	m_iListSize++;

	return 0;
}

template <typename T>
bool myList<T>::outHead(T &data)
{
	if (isEmpty())
	{
		cout << "The list is empty now!" << endl;
		return -1;
	}

	listNode<T> *temp;

	if (1 == m_iListSize)
	{
		data = *(m_pHead->m_pData);
		temp = m_pHead;
		delete temp;
		m_pHead = NULL;
		m_pTail = NULL;
	}
	else
	{
		m_pTail->m_pNext = m_pHead->m_pNext;
		m_pHead->m_pNext->m_pPre = m_pHead->m_pPre;

		data = *(m_pHead->m_pData);
		temp = m_pHead;
		m_pHead = m_pHead->m_pNext;
		delete temp;
	}
	
	m_iListSize--;
	return 0;
}

template <typename T>
bool myList<T>::add(int index, T &data)
{
	if (index > m_iListSize || index <= 0)
	{
		cout << "Your index is out of the range!" << endl;
		return -1;
	}

	if (isFull())
	{
		cout << "The list is full, size is " << m_iListSize << endl;
		return -1;
	}

	listNode<T> *temp = m_pHead;
	listNode<T> *pNode = new listNode<T>(&data);

	if (isEmpty())
	{
		cout << "Frist input" << endl;
		m_pHead = pNode;
		m_pTail = pNode;
		m_iListSize++;
		return 0;
	}

	if (index == m_iListSize)
	{
		addTail(data);
		return 0;
	}

	for(int i = 1; i < index; i++)
	{
		temp = temp->m_pNext;
	}

	pNode->m_pNext = temp->m_pNext;
	pNode->m_pPre = temp;
	temp->m_pNext->m_pPre = pNode;
	temp->m_pNext = pNode;

	m_iListSize++;

	return 0;
}

template <typename T>
bool myList<T>::out(int index, T &data)
{
	if (index > m_iListSize || index <= 0)
	{
		cout << "Your index is out of the range!" << endl;
		return -1;
	}

	if (isEmpty())
	{
		cout << "The list is empty now!" << endl;
		return -1;
	}

	if (index == 1)
	{
		outHead(data);
		return 0;
	}

	if (index == m_iListSize)
	{
		m_pTail = m_pTail->m_pPre;
	}

	listNode<T> *temp = m_pHead;

	for (int i = 1; i < index; i++)
	{
		temp = temp->m_pNext;
	}

	data = *(temp->m_pData);
	temp->m_pPre->m_pNext = temp->m_pNext;
	temp->m_pNext->m_pPre = temp->m_pPre;

	delete temp;

	m_iListSize--;
	return 0;
}

template <typename T>
bool myList<T>::isEmpty() 
{
	return m_iListSize == 0 ? true : false;
}

template <typename T>
bool myList<T>::isFull()
{
	return m_iListSize == m_iListCapacity ? true : false;
}

template <typename T>
void myList<T>::listPrint()
{
	if (isEmpty())
	{
		cout << "The list is empty, please input element first." << endl;
		return;
	}

	listNode<T> *temp = m_pHead;
	for (int i = 0; i < m_iListSize; i++)
	{
		cout << *(temp->m_pData) << endl;
		temp = temp->m_pNext;
	}
}

#endif // !__MY_LIST_H__