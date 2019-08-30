#ifndef __BREAKROAD_H__
#define __BREAKROAD_H__

#include <iostream>
#include <string>

using namespace std;

template <typename T>
class breakRoad
{
public:
	breakRoad(int length);
	~breakRoad();

	bool push(T data);
	bool pop(T &data);

	bool isEmpty();
	bool isFull();
	int	 length();
	
	void clear();
	void show();

private:
	int m_iStackLength;
	int m_iStackTop;
	int m_iStackBottom;

	T* m_pStarkList;
};


template <typename T>
breakRoad<T>::breakRoad(int length)
{
	if (0 >= length)
	{
		cout << "The length of stack should be bigger than 0" << endl;
	}
	else
	{
		clear();
		m_iStackLength = length;
		m_pStarkList = new T[length];
	}
}

template <typename T>
breakRoad<T>::~breakRoad()
{
	clear();
	delete[]m_pStarkList;
	m_pStarkList = NULL;
}

template <typename T>
bool breakRoad<T>::push(T data)
{
	if (isFull())
	{
		cout << "The stack is FULL now!" << endl;
		return false;
	}

	m_pStarkList[++m_iStackTop] = data;

	return true;
}

template <typename T>
bool breakRoad<T>::pop(T &data)
{
	if (isEmpty())
	{
		cout << "This stack is EMPTY now!" << endl;
		return false;
	}

	data = m_pStarkList[m_iStackTop--];
	return true;
}

template <typename T>
bool breakRoad<T>::isEmpty()
{
	return m_iStackTop == -1 ? true : false;
}

template <typename T>
bool breakRoad<T>::isFull()
{
	return m_iStackTop+1 == m_iStackLength ? true : false;
}

template<typename T>
int breakRoad<T>::length()
{
	return m_iStackLength;
}

template <typename T>
void breakRoad<T>::clear()
{
	m_iStackBottom = 0;
	m_iStackTop = -1;
	m_iStackLength = 0;
}

template <typename T>
void breakRoad<T>::show()
{
	if (isEmpty())
	{
		cout << "The stack is empty, please push data to it." << endl;
		return;
	}
	cout << "Show: ";
	for (int i = 0; i <= m_iStackTop; i++)
	{
		cout << m_pStarkList[i] << " ";
	}

	cout << endl;
}

#endif // !__BREAKROAD_H__