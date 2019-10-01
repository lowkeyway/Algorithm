#include "MySearch.h"

#define LEN(ARRAY) sizeof(ARRAY)/sizeof(ARRAY[0])

const int MyArray[] = { 0, 2, 4, 6, 8, 10, 9, 7, 5, 3, 1};


int main()
{
	int index = -1;
	MySearch S(MyArray, LEN(MyArray));

	S.sequenceSearch(10, index);
	cout << "index = " << index << endl;

	index = -1;
	S.binarySearch(10, index);
	cout << "index = " << index << endl;

	index = -1;
	S.insertSearch(10, index);
	cout << "index = " << index << endl;

	system("pause");
	return true;
}