#include <iostream>
#include <string>
#include "seniority.h"
#include "treeNode.h"
#include "binTree.h"

using namespace std;

/***************
					   0[Grandpa, 80]
				/						\
		1[Father, 55]					2[Aunt, 50]
		/			\					/			\
3[lowkeyway, 30]	4[Sister, 32]	5[cousin, 30]

Preorder:	root->left->right	0 1 3 4 2 5
Inorder:	left->root->right	3 1 4 0 5 2
Postorder:	left->right->root	3 4 1 5 2 0


***************/


int main()
{
	seniority s0("Grandpa", 80);
	seniority s1("Father", 55);
	seniority s2("Aunt", 50);
	seniority s3("lowkeyway", 30);
	seniority s4("Sister", 32);
	seniority s5("cousin", 30);

	treeNode<seniority> t0(0, s0);
	treeNode<seniority> t1(1, s1);
	treeNode<seniority> t2(2, s2);
	treeNode<seniority> t3(3, s3);
	treeNode<seniority> t4(4, s4);
	treeNode<seniority> t5(5, s5);

	t0.nodePrint();

	binTree<seniority> bt(&t0);
	bt.treeAdd(0, LEFT, &t1);
	bt.treeAdd(0, RIGHT, &t2);
	bt.treeAdd(1, LEFT, &t3);
	bt.treeAdd(1, RIGHT, &t4);
	bt.treeAdd(2, LEFT, &t5);

	//bt.treeDel(1);
	

	cout << "--------preorderPrint--------" << endl;
	bt.preorderPrint();
	cout << "--------inorderPrint--------" << endl;
	bt.inorderPrint();
	cout << "--------postorderPrint--------" << endl;
	bt.postorderPrint();


	system("pause");
	return 0;
}