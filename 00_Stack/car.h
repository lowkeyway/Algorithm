#ifndef __CAR_H__
#define __CAR_H__

#include <iostream>
#include <string>

using namespace std;

class car
{
	friend ostream &operator << (ostream &out, car &Car);
public:
	car(string name = "car", int speed = 10);
	~car();



public:
	string m_sName;
	int m_iSpeed;
};

#endif