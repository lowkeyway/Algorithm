#include "hashTable.h"

int main()
{
	string score;

	hashTable h(200);
	h.add("louis", "100");
	h.add("Willis", "99");
	h.add("John", "98");
	h.add("John", "97");

	h.get("John", score);
	cout << score << endl;

	h.del("John", "99");
	h.get("John", score);
	cout << score << endl;

	system("pause");
	return true;
}