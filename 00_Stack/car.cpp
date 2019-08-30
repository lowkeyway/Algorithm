#include "car.h"

car::car(string name, int speed)
{
	m_sName = name;
	m_iSpeed = speed;
}

car::~car()
{
}

ostream &operator << (ostream &out, car &Car)
{
	out << "[" << Car.m_sName << ", " << Car.m_iSpeed << "]";
	return out;
}