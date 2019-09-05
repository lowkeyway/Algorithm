#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;


#define ull					unsigned long long
#define uint				unsigned int
#define STRING_TO_HASHNUM	131
#define HASH_MODULO			101
#define P_A					3
#define P_B					7

typedef struct node
{
	string key;
	string value;
	struct node* pNext;
}Node;

inline ull stringHashKey(string s)
{
	ull sum = 0;
	for (int i = 0; i < s.size(); i++)
	{
		sum = sum * STRING_TO_HASHNUM + (ull)s[i];
	}

	return sum;
}

inline void __geValue(Node* pN, string &_value)
{
	if (NULL == pN)
	{
		return;
	}

	_value += pN->value;

	if (NULL == pN->pNext)
	{
		return;
	}

	_value = _value + ", ";

	__geValue(pN->pNext, _value);

	return;
}

inline uint findMinPrime(int _key)
{
	uint temp;
	uint indexMax;
	temp = (uint)_key;
	bool isPrime = true;

	while (true)
	{
		isPrime = true;
		indexMax = (uint)sqrt(temp);
		for (uint i = 2; i <= indexMax; i++)
		{
			if (0 == temp % i)
			{
				isPrime = false;
				temp++;
				break;
			}
		}

		if (true == isPrime)
		{
			return temp;
		}
	}
}

class hashTable
{
public:
	hashTable(int size = HASH_MODULO);
	~hashTable() {};

	bool add(string _key, string _value);
	bool add(Node* pN);

	Node* get(string _key, string &_value);

	bool del(string _key);
	bool del(string _key, string _value);

private:
	uint tableCap;
	vector<Node*> hashList;
	uint hashFun(ull stringKey);
};

#endif