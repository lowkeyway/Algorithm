#include "Graph.h"


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
}

Graph::~Graph()
{
	delete[]m_pNodeArray;
	delete[]m_piMatrix;
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