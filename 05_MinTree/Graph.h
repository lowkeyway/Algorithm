#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <iostream>
#include <string>
#include <vector>
#include "Node.h"
#include "Edge.h"

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

	int	 getMinWeightEdgeIndex(vector<Edge> &vEdge);
	bool primTree(int nodeIndex);

	bool kruskalTree();

private:
	int m_iGraphCap;
	int m_iNodeCount;
	Node* m_pNodeArray;
	int* m_piMatrix;

	Edge* m_pEdge;
};
#endif // !__GRAPH_H__
