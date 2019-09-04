#ifndef __EDGE_H__
#define __EDGE_H__

#include <iostream>

using namespace std;

class Edge
{
public:
	Edge(int nodeIndex1=0, int nodeIndex2=0, int weight=0);
	~Edge() {};

public:
	int m_iNodeIndexBegin;
	int m_iNodeIndexEnd;
	int m_iWeight;
	bool m_bIsSelect;
};



#endif // !__EDGE_H__
