#include <iostream>
#include <string>

#include "seniority.h"

using namespace std;

seniority::seniority(string sen, int age)
{
	m_sSen = sen;
	m_iAge = age;
}

ostream &operator << (ostream &out, seniority &Sen)
{
	out << "Seniority: " << Sen.m_sSen << ", Age is " << Sen.m_iAge;
	return out;
}