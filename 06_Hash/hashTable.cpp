#include "hashTable.h"


hashTable::hashTable(int size)
{
	if (size <= 0)
	{
		cout << "Please input correct number!" << endl;
		return;
	}

	tableCap = findMinPrime(size);
	hashList.resize(tableCap);
	for (uint i = 0; i < tableCap; i++)
	{
		hashList[i] = NULL;
	}
}

bool hashTable::add(string _key, string _value)
{
	Node* pN = new Node;
	pN->key = _key;
	pN->value = _value;
	pN->pNext = NULL;
	add(pN);
	return true;
}

bool hashTable::add(Node* pN)
{
	ull hashKey = stringHashKey(pN->key);
	uint _key = hashFun(hashKey);

	pN->pNext = hashList[_key];
	hashList[_key] = pN;

	return true;
}

uint hashTable::hashFun(ull stringKey)
{
	//return (uint)(stringKey % HASH_MODULO);
	return (uint)((P_A * stringKey + P_B) % tableCap);
}

Node* hashTable::get(string _key, string &_value)
{
	ull hashKey = stringHashKey(_key);
	uint _hashKey = hashFun(hashKey);
	_value.clear();

	if (NULL == hashList[_hashKey])
	{
		cout << "There is no this key!" << endl;
		return NULL;
	}

	__geValue(hashList[_hashKey], _value);

	return hashList[_hashKey];
}


bool hashTable::del(string _key)
{
	Node *pTemp;
	ull hashKey = stringHashKey(_key);
	uint _hashKey = hashFun(hashKey);
	if (NULL == hashList[_hashKey])
	{
		cout << "There is no this key!" << endl;
		return false;
	}

	do
	{
		pTemp = hashList[_hashKey];
		hashList[_hashKey] = hashList[_hashKey]->pNext;
		pTemp->pNext = NULL;
		delete pTemp;
		pTemp = NULL;
	} while (NULL != hashList[_hashKey]);

	return true;
}

bool hashTable::del(string _key, string _value)
{
	Node *pDel;
	Node **ppTemp;
	ull hashKey = stringHashKey(_key);
	uint _hashKey = hashFun(hashKey);
	if (NULL == hashList[_hashKey])
	{
		cout << "There is no this key!" << endl;
		return false;
	}

	pDel = hashList[_hashKey];
	ppTemp = &hashList[_hashKey];

	do
	{
		if (NULL == pDel)
		{
			cout << "Cannot find this value" << endl;
			return false;
		}

		if (pDel->value == _value)
		{
			*ppTemp = pDel->pNext;
			pDel->pNext = NULL;
			delete pDel;
			pDel = NULL;

			cout << "Delete Success!" << endl;
			return true;
		}

		pDel = pDel->pNext;
		ppTemp = &((*ppTemp)->pNext);
	} while (true);
}