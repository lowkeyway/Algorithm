#ifndef __BIN_TREE_H__
#define __BIN_TREE_H__

#include <string>
#include <iostream>

#ifndef SWAP(X, Y)
#define SWAP(X, Y) (X)=(X)+(Y);(Y)=(X)-(Y);(X)=(X)-(Y)
#endif // !SWAP(X, Y)



using namespace std;

typedef enum BigLittle
{
	BIG = 0,
	LITTLE = 1
}BIG_LITTLE;

class BinTree
{
public:
	BinTree(int* p, int cap);
	~BinTree();
	bool buildHeap(BIG_LITTLE bl);
	bool showBinTree();

public:
	int* m_piBinTree;
	int  m_iCap;
	int  m_iCount;
};


#endif // !__BIN_TREE_H__
