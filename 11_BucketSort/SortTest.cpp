#include "MySort.h"
#include "binTree.h"

#define LEN(X) (sizeof(X)/sizeof(X[0]))

using namespace std;


int myArray[] = { 1, 3, 5, 7, 9, 10, 8, 6, 4, 2 };

int main()
{
	MySort M(myArray, LEN(myArray));
	M.bubbleSort(DOWN);
	M.fasteSort(DOWN);
	M.binTreeSort(DOWN);
	M.countSort(DOWN);
	M.bucketSort(DOWN);

	//BinTree B(myArray, LEN(myArray));
	//B.showBinTree();
	//B.buildHeap(BIG);
	//B.showBinTree();
	//B.buildHeap(LITTLE);
	//B.showBinTree();
	system("pause");
	return true;
}