#include <iostream>
#include <string>
#include "myList.h"
#include "car.h"

using namespace std;
static bool listTest();

int main()
{
	listTest();
	system("pause");
	return 0;
}

static bool listTest()
{
	myList<int> ml(10);
	int data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int out[10] = { 0 };

	ml.addTail(data[0]);
	ml.addTail(data[1]);
	ml.addTail(data[2]);
	ml.addTail(data[3]);
	ml.addTail(data[4]);
	ml.add(4, data[5]);

	ml.listPrint();

	ml.out(4, out[0]);
	ml.outHead(out[1]);
	ml.outHead(out[2]);
	ml.outHead(out[3]);
	ml.outHead(out[4]);
	ml.outHead(out[5]);

	cout << "out data[0]: " << out[0] << endl;
	
	ml.listPrint();

	
	myList<car> mc(10);
	car c1("car1", 10);
	car c2("car2", 20);
	car c3("car3", 30);
	car c4("car4", 40);
	car c5("car5", 50);

	car c6, c7, c8, c9, c10;

	mc.addTail(c1);
	mc.addTail(c2);
	mc.addTail(c3);
	mc.addTail(c4);
	mc.addTail(c5);

	mc.listPrint();

	mc.outHead(c6);
	mc.outHead(c7);

	cout << "out car is :" << c6 << c7 << endl;
	mc.listPrint();





	return 0;
}