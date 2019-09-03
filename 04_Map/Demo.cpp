#include <iostream>
#include <string>
#include "Graph.h"

using namespace std;


/*
Graph Example
				A
		/			   \
	   B				C
	/		\		/		\
   D		E		F	-	 G
	\	   /
		H
*/

/*
Matrix Example
	A  B  C  D  E  F  G  H
A   0  1  1  0  0  0  0  0
B   1  0  0  1  1  0  0  0
C   1  0  0  0  0  1  1  0
D   0  1  0  0  0  0  0  1
E   0  1  0  0  0  0  0  1
F   0  0  1  0  0  0  1  0
G   0  0  1  0  0  1  0  0
H   0  0  0  1  1  0  0  0
*/

int main()
{
	Node N0('A');
	Node N1('B');
	Node N2('C');
	Node N3('D');
	Node N4('E');
	Node N5('F');
	Node N6('G');
	Node N7('H');

	Graph G(8);
	G.addNode(&N0);
	G.addNode(&N1);
	G.addNode(&N2);
	G.addNode(&N3);
	G.addNode(&N4);
	G.addNode(&N5);
	G.addNode(&N6);
	G.addNode(&N7);

	G.printMatrix();


	G.setUndirectedMat(1, 0);
	G.setUndirectedMat(2, 0);
	G.setUndirectedMat(3, 1);
	G.setUndirectedMat(4, 1);
	G.setUndirectedMat(5, 2);
	G.setUndirectedMat(6, 2);
	G.setUndirectedMat(6, 5);
	G.setUndirectedMat(7, 3);
	G.setUndirectedMat(7, 4);

	G.printMatrix();

	G.resetNode();
	cout << "DFT:	";
	G.deepFristTraverse(0);
	cout << endl;

	G.resetNode();
	cout << "BFT:	";
	G.breadthFristTraverse(0);
	cout << endl;
	

	system("pause");
	return true;
}