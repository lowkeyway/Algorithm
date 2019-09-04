#include <iostream>
#include <string>
#include "Graph.h"

using namespace std;


/*
Graph Example
			A
		/	|	\
		B-	F	-E
		\	|	/
		C	-	D

A	B	C	D	E	F
0	1	2	3	4	5

    A  B  C  D  E  F
A   0  6  0  0 10  7
B   6  0  3  0  0  5
C   0  3  0  8  0  9
D   0  0  8  0  4  2
E  10  0  0  4  0  1
F   7  5  9  2  1  0

*/


/*
Weight
A-B	6	A-F 7	A-E 10
B-C 3	B-F 5
C-D 8	C-F 9
D-E 4	D-F 2
E-F 1
*/

int main()
{
	Node N0('A');
	Node N1('B');
	Node N2('C');
	Node N3('D');
	Node N4('E');
	Node N5('F');


	Graph G(6);
	G.addNode(&N0);
	G.addNode(&N1);
	G.addNode(&N2);
	G.addNode(&N3);
	G.addNode(&N4);
	G.addNode(&N5);

	G.printMatrix();

	G.setUndirectedMat(1, 0, 6);
	G.setUndirectedMat(2, 1, 3);
	G.setUndirectedMat(3, 2, 8);
	G.setUndirectedMat(4, 0, 10);
	G.setUndirectedMat(4, 3, 4);
	G.setUndirectedMat(5, 0, 7);
	G.setUndirectedMat(5, 1, 5);
	G.setUndirectedMat(5, 2, 9);
	G.setUndirectedMat(5, 3, 2);
	G.setUndirectedMat(5, 4, 1);


	G.resetNode();
	G.breadthFristTraverse(0);
	cout << endl;

	G.resetNode();
	G.deepFristTraverse(0);
	cout << endl;

	G.resetNode();
	G.primTree(0);
	
	G.resetNode();
	G.kruskalTree();

	system("pause");
	return true;
}