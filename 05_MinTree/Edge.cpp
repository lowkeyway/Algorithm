#include "Edge.h"

Edge::Edge(int begin, int end, int weight)
{
	m_iNodeIndexBegin = begin;
	m_iNodeIndexEnd = end;
	m_iWeight = weight;
	m_bIsSelect = false;
}