#include <iostream>
#include <string>
#include "Node.h"

using namespace std;

Node::Node(char data)
{
	m_cData = data;
	m_bIsVisited = false;
}