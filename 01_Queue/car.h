#ifndef __CAR_H__
#define __CAR_H__

#include <iostream>
#include <ostream>
#include <string>

using namespace std;

class car
{
	friend ostream &operator << (ostream &out, car &Car);
public:
	car() {};
	car(string name, int speed);
	~car(){};

private:
	string m_sName;
	int m_iSpeed;
};

#endif