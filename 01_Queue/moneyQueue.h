#ifndef __MONEY_QUEUE_H__
#define __MONEY_QUEUE_H__

#include <iostream>
#include <string>

using namespace std;

template <typename T>
class moneyQueue
{
public:
	moneyQueue(int size);
	~moneyQueue();
	bool clear();
	bool add(T t);
	bool out(T &t);
	bool isEmpty();
	bool isFull();
	int  size();

	bool show();
private:
	T* m_pQueue;
	int m_iTop;
	int m_iBottom;
	int m_iSize;

};

template <typename T>
moneyQueue<T>::moneyQueue(int size)
{
	if (0 >= size)
	{
		cout << "Wrong queue number!" << endl;
		return;
	}
	clear();
	m_iSize = size;
	m_pQueue = new T[size];
}


template <typename T>
moneyQueue<T>::~moneyQueue()
{
	clear();
	delete []m_pQueue;
	m_pQueue = NULL;
}

template <typename T>
bool moneyQueue<T>::clear()
{
	m_iBottom = -1;
	m_iTop = 0;
	m_iSize = 0;
	return true;
}

template <typename T>
bool moneyQueue<T>::add(T t)
{
	if (isFull())
	{
		cout << "The queue is full!" << endl;
		return false;
	}

	m_pQueue[++m_iBottom] = t;
	return true;
}

template <typename T>
bool moneyQueue<T>::out(T &t)
{
	if (isEmpty())
	{
		cout << "The queue is empty!" << endl;
		return false;
	}

	t = m_pQueue[0];
	for(int i = 0; i < m_iBottom; i++)
	{
		m_pQueue[i] = m_pQueue[i + 1];
	}

	m_iBottom--;
	return true;
}

template <typename T>
bool moneyQueue<T>::isEmpty()
{
	return m_iBottom == -1 ? true : false;
}


template <typename T>
bool moneyQueue<T>::isFull()
{
	return m_iBottom + 1 == m_iSize ? true : false;
}


template <typename T>
int moneyQueue<T>::size()
{
	return m_iBottom;
}

template<typename T>
bool moneyQueue<T>::show()
{
	if (isEmpty())
	{
		cout << "The queue is empty, nothing to show!" << endl;
		return false;
	}

	for (int i = 0; i <= m_iBottom; i++)
	{
		cout << m_pQueue[i] << " " ;
	}

	cout << endl;
	return true;
}


#endif // !1
