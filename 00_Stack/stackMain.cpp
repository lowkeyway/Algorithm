#include <iostream>
#include <string>
#include "breakRoad.h"
#include "car.h"

using namespace std;

static bool myStack();

int main()
{
	myStack();

	system("pause");
	return 0;
}

static bool myStack()
{
	breakRoad<int> b(5);
	breakRoad<string> s(5);
	breakRoad<car> c(5);

	b.push(1);
	b.push(2);
	b.push(3);
	b.push(4);
	b.push(5);
	b.show();

	int iData = -1;
	b.pop(iData);
	cout << "INT POP data is: " << iData << endl;
	b.show();
	cout << endl << endl;


	s.push("H");
	s.push("e");
	s.push("l");
	s.push("l");
	s.push("o");
	s.show();
	string sData = "";
	s.pop(sData);
	cout << "STRING POP data is: " << sData << endl;
	s.show();
	cout << endl << endl;

	car c1("car1", 10);
	car c2("car2", 20);
	car c3("car3", 30);
	car c4("car4", 40);
	car c5("car5", 50);
	car c6("car6", 60);
	c.push(c1);
	c.push(c2);
	c.push(c3);
	c.push(c4);
	c.push(c5);
	c.push(c6);
	c.show();

	car cc;
	c.pop(cc);
	cout << "STRING POP data is: " << cc << endl;
	c.show();
	cout << endl << endl;

	return true;
}