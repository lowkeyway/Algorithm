#include <iostream>
#include <string>
#include "moneyQueue.h"
#include "car.h"

using namespace std;

static void myQueue();

int main()
{
	myQueue();
	system("pause");
	return 0;
}

static void myQueue()
{
	moneyQueue<int> i(5);
	moneyQueue<string> s(5);
	moneyQueue<car> c(5);

	i.add(1);
	i.add(2);
	i.add(3);
	i.add(4);
	i.add(5);
	i.show();
	int iOut;
	i.out(iOut);
	cout << "iOut is " << iOut << endl;

	i.show();


	s.add("H");
	s.add("e");
	s.add("l");
	s.add("l");
	s.add("o");
	s.show();
	string sOut;
	s.out(sOut);
	cout << "sOut is " << sOut << endl;
	s.add("!");
	s.show();


	car c1("car1", 10);
	car c2("car2", 20);
	car c3("car3", 30);
	car c4("car4", 40);
	car c5("car5", 50);
	car c6("car6", 60);
	car c7("car7", 70);
	//car c7;

	c.add(c1);
	c.add(c2);
	c.add(c3);
	c.add(c4);
	c.add(c5);
	c.add(c6);
	c.show();
	c.out(c7);
	cout << "out data is " << c7 << endl;

	c.show();
}