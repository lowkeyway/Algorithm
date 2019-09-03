#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <iostream>
#include <string>
#include <vector>
#include "Node.h"

using namespace std;

class Graph
{
public:
	Graph(int capacity);
	~Graph();
	bool addNode(Node* pNode);
	bool resetNode();
	bool setUndirectedMat(int row, int col, int weight = 1);
	bool getUndirectedMat(int row, int col, int &val);
	bool printMatrix();
	bool deepFristTraverse(int nodeIndex);
	bool breadthFristTraverse(int nodeIndex);
	bool breadthFristTraverseImp(vector<int> V);

private:
	int m_iGraphCap;
	int m_iNodeCount;
	Node* m_pNodeArray;
	int* m_piMatrix;
};
#endif // !__GRAPH_H__
