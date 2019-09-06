#include "MySort.h"


#define LEN(X) (sizeof(X)/sizeof(X[0]))

using namespace std;


int myArray[] = { 1, 3, 5, 7, 9, 10, 8, 5, 4, 2 };

int main()
{
	MySort M(myArray, LEN(myArray));
	M.bubbleSort(DOWN);
	

	system("pause");
	return true;
}