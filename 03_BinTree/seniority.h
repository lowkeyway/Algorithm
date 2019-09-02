#ifndef __SENIORITY_H__
#define __SENIORITY_H__

#include <iostream>
#include <ostream>
#include <string>

using namespace std;

class seniority
{
	friend ostream &operator << (ostream &out, seniority &sen);
public:
	seniority(string sen = "", int age = 0);
	~seniority() {};

private:
	string	m_sSen;
	int		m_iAge;
};

#endif // !__SENIORITY_H__
