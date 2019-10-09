#ifndef __MY_BIN_TREE_H__
#define __MY_BIN_TREE_H__

enum RBTColor
{
	RED,
	BLACK
};

enum RBTDirection
{
	LEFT,
	RIGHT
};

typedef struct binTreeData {
	int data;
	int index;
}BinTreeData;

typedef struct binTreeNode {
	struct binTreeNode *pLeft;
	struct binTreeNode *pRight;
	struct binTreeNode *pParent;
	int height;
	RBTColor Color;
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


	bool creatAvlTree(int* pData, int iLen);
	bool creatRBTree(int* pData, int iLen);

public:
	BinTreeNode* m_pHead;

private:
	int getHeight(BinTreeNode *pTree);

	BinTreeNode * LLRotation(BinTreeNode *pN0);
	BinTreeNode * RRRotation(BinTreeNode *pN0);
	BinTreeNode * LRRotation(BinTreeNode *pN0);
	BinTreeNode * RLRotation(BinTreeNode *pN0);
	bool insertAvlTree(BinTreeNode *&pParent, BinTreeNode *pInsert);
	bool insertRBTree(BinTreeNode *&pParent, BinTreeNode *pInsert);
};


#endif // !__MY_BIN_TREE_H__
