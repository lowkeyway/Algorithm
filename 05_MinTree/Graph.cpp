#include "Graph.h"
#include <algorithm>

Graph::Graph(int capacity)
{
	m_iGraphCap = capacity;
	m_iNodeCount = 0;

	m_pNodeArray = new Node[capacity];
	if (NULL == m_pNodeArray)
	{
		cout << "m_pNodeArray malloc fail" << endl;
		return;
	}
	memset(m_pNodeArray, 0, sizeof(Node)*capacity);

	m_piMatrix = new int[capacity*capacity];
	if (NULL == m_piMatrix)
	{
		cout << "m_piMatrix malloc fail" << endl;
		return;
	}
	memset(m_piMatrix, 0, sizeof(int)*capacity*capacity);

	m_pEdge = new Edge[capacity - 1];
	if (NULL == m_pEdge)
	{
		cout << "m_pEdge malloc fail" << endl;
		return;
	}
	memset(m_pEdge, 0, sizeof(Edge)*(capacity - 1));
}

Graph::~Graph()
{
	delete[]m_pNodeArray;
	delete[]m_piMatrix;
	delete[]m_pEdge;
	m_iGraphCap = 0;
	m_iNodeCount = 0;
}

bool Graph::addNode(Node* pNode)
{
	if (NULL == pNode)
	{
		cout << "Input Node is NULL" << endl;
		return false;
	}

	if (m_iNodeCount > m_iGraphCap)
	{
		cout << "Graph is FULL";
		return false;
	}

	m_pNodeArray[m_iNodeCount].m_cData = pNode->m_cData;
	m_iNodeCount++;
	return true;
}

bool Graph::resetNode()
{
	for (int i = 0; i < m_iGraphCap; i++)
	{
		m_pNodeArray[i].m_bIsVisited = false;
	}
	return true;
}

bool Graph::setUndirectedMat(int row, int col, int weight)
{
	if (row > m_iGraphCap || row < 0 || col > m_iGraphCap || col < 0)
	{
		cout << "Input Coordinate is out of range!" << endl;
		return false;
	}

	m_piMatrix[col*m_iGraphCap + row] = weight;
	m_piMatrix[row*m_iGraphCap + col] = weight;

	return true;
}

bool Graph::getUndirectedMat(int row, int col, int &val)
{
	if (row > m_iGraphCap || row < 0 || col > m_iGraphCap || col < 0)
	{
		cout << "Input Coordinate is out of range!" << endl;
		return false;
	}

	val = m_piMatrix[col*m_iGraphCap + row];
	return true;
}

bool Graph::printMatrix()
{
	for (int col = 0; col < m_iGraphCap; col++)
	{
		for (int row = 0; row < m_iGraphCap; row++)
		{
			cout << m_piMatrix[col*m_iGraphCap + row] << "  ";
		}
		cout << endl;
	}
	cout << "-----------------------------------" << endl;
	return true;
}

//DFT
bool Graph::deepFristTraverse(int nodeIndex)
{
	int weight = 0;
	if (nodeIndex < 0 || nodeIndex > m_iGraphCap)
	{
		cout << "Out of range!" << endl;
		return false;
	}

	cout << m_pNodeArray[nodeIndex].m_cData << " ";
	m_pNodeArray[nodeIndex].m_bIsVisited = true;

	for (int col = 0; col < m_iGraphCap; col++)
	{
		getUndirectedMat(nodeIndex, col, weight);
		if (weight == 0)
		{
			continue;
		}

		if (true == m_pNodeArray[col].m_bIsVisited)
		{
			continue;
		}

		deepFristTraverse(col);
	}

	return true;
}

//BFT
bool Graph::breadthFristTraverse(int nodeIndex)
{
	int weight = 0;
	if (nodeIndex < 0 || nodeIndex > m_iGraphCap)
	{
		cout << "Out of range!" << endl;
		return false;
	}
	
	cout << m_pNodeArray[nodeIndex].m_cData << " ";
	m_pNodeArray[nodeIndex].m_bIsVisited = true;

	vector<int> V;
	V.push_back(nodeIndex);
	breadthFristTraverseImp(V);

	return true;
}

bool Graph::breadthFristTraverseImp(vector<int> V)
{
	int weight;
	vector<int> curV;

	if (V.empty())
	{
		return true;
	}

	for (vector<int>::iterator curIndex = V.begin(); curIndex != V.end(); curIndex++)
	{
		for (int col = 0; col < m_iGraphCap; col++)
		{
			getUndirectedMat(*curIndex, col, weight);
			if (0 == weight)
			{
				continue;
			}

			if (true == m_pNodeArray[col].m_bIsVisited)
			{
				continue;
			}

			cout << m_pNodeArray[col].m_cData << " ";
			m_pNodeArray[col].m_bIsVisited = true;
			curV.push_back(col);
		}
	}

	breadthFristTraverseImp(curV);

	return true;
}

int	 Graph::getMinWeightEdgeIndex(vector<Edge> &vEdge)
{
	Edge *pMinEdge = NULL;
	int tempEdgeIndex = -1;

	for (int i = 0; i < (int)vEdge.size(); i++)
	{
		if (true == vEdge[i].m_bIsSelect)
		{
			continue;
		}

		if (NULL == pMinEdge)
		{
			pMinEdge = &vEdge[i];
			tempEdgeIndex = i;
		}
		else
		{
			if (vEdge[i].m_iWeight < pMinEdge->m_iWeight)
			{
				pMinEdge = &vEdge[i];
				tempEdgeIndex = i;
			}
		}

	}

	if (NULL == pMinEdge)
	{
		cout << "getMinWeightNode Fail!" << endl;
		return -1;
	}


	return tempEdgeIndex;
}


bool Graph::primTree(int nodeIndex)
{
	vector<int> vNode;
	vector<Edge> vEdge;
	int weight = 0;
	int edgeCount = 0;
	int tempNodeIndex = 0;
	int tempEdgeIndex = 0;

	vNode.push_back(nodeIndex);
	m_pNodeArray[nodeIndex].m_bIsVisited = true;
	cout << m_pNodeArray[nodeIndex].m_cData << endl;

	while (edgeCount < m_iGraphCap - 1)
	{
		tempNodeIndex = vNode.back();
		for (int col = 0; col < m_iGraphCap; col++)
		{
			getUndirectedMat(tempNodeIndex, col, weight);
			if (0 == weight)
			{
				continue;
			}

			if (true == m_pNodeArray[col].m_bIsVisited)
			{
				continue;
			}

			Edge E(tempNodeIndex, col, weight);
			vEdge.push_back(E);
		}

		tempEdgeIndex = getMinWeightEdgeIndex(vEdge);
		m_pEdge[edgeCount] = vEdge[tempEdgeIndex];
		cout << "----" << vEdge[tempEdgeIndex].m_iWeight << "----" << endl;
		vEdge[tempEdgeIndex].m_bIsSelect = true;


		tempNodeIndex = vEdge[tempEdgeIndex].m_iNodeIndexEnd;
		vNode.push_back(tempNodeIndex);
		m_pNodeArray[tempNodeIndex].m_bIsVisited = true;
		cout << m_pNodeArray[tempNodeIndex].m_cData << endl;
		edgeCount++;
	}

	return true;
}

//Cannot be a class function, the para number shoud be 2. NO SELF.
bool sortFunc(const Edge &E1, const Edge &E2)
{
	return E1.m_iWeight < E2.m_iWeight;
}

int findIndex(int* pNodeIndex, int index)
{
	while (-1 != pNodeIndex[index])
	{
		index = pNodeIndex[index];
	}

	return index;
}


bool Graph::kruskalTree()
{
	vector<Edge> vEdge;
	int weight = 0;
	int begain = 0;
	int end = 0;

	int* pNodeIndex = new int[m_iGraphCap];
	if (NULL == pNodeIndex)
	{
		cout << "Malloc pNode fail!" << endl;
		return false;
	}
	memset(pNodeIndex, -1, sizeof(int)*m_iGraphCap);

	//1. Find all edges.
	for (int row = 0; row < m_iGraphCap; row++)
	{
		for (int col = 0; col < row; col++)
		{
			getUndirectedMat(row, col, weight);
			if (0 == weight)
			{
				continue;
			}

			Edge E(row, col, weight);
			vEdge.push_back(E);
		}
	}

	//2. Up sort them by weight.
	sort(vEdge.begin(), vEdge.end(), sortFunc);

	//3. kruskalTree table.
	for (int i = 0; i < (int)vEdge.size(); i++)
	{
		begain = findIndex(pNodeIndex, vEdge[i].m_iNodeIndexBegin);
		end = findIndex(pNodeIndex, vEdge[i].m_iNodeIndexEnd);

		if (begain != end)
		{
			pNodeIndex[begain] = end;
			cout << m_pNodeArray[vEdge[i].m_iNodeIndexBegin].m_cData  << "----(" << vEdge[i].m_iWeight << ")----"\
				<< m_pNodeArray[vEdge[i].m_iNodeIndexEnd].m_cData << endl;
		}
	}
	
	delete[]pNodeIndex;
	return true;
}