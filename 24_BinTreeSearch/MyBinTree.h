#ifndef __MY_BIN_TREE_H__
#define __MY_BIN_TREE_H__

typedef struct binTreeData {
	int data;
	int index;
}BinTreeData;

typedef struct binTreeNode {
	struct binTreeNode *pLeft;
	struct binTreeNode *pRight;
	BinTreeData node;
}BinTreeNode;

class BinTree
{
public:
	BinTree();
	~BinTree();

	bool deleteNode(BinTreeNode *pNode);
	bool creatSearchTree(int* pData, int iLen);
	bool searchSearchTree(int target, int &index);

public:
	BinTreeNode* m_pHead;
};


#endif // !__MY_BIN_TREE_H__
