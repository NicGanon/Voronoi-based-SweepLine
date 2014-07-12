#pragma once
#include "BeachNode.h"
#include "EventNode.h"
class BeachLine
{
public:
	BeachLine():root(NULL){};
	~BeachLine(){};

	void LeftRotation(BeachNode* subTree);
	void RightRotation(BeachNode* subTree);
	
	void Insert(BeachNode*pos,BeachNode* subTree);
	void InsertFixup(BeachNode *newNode);
	
	BeachNode* FindArc( Point &site);
	BeachNode* BuildSubTree(BeachNode*arc, Point &site);
	BeachNode* BeachLine::BuildSubBeachLine (Point &leftArcSite, Point &rightArcSite);

	int GetHeight(BeachNode *subTree);
	void DeleteFixup(BeachNode *fixNode);
 
	bool Empty();
	void SetRoot (BeachNode *r) { root = r;    }
	BeachNode* GetRoot()        { return root; }

	void InorderTrave() {Inorder(root);};
	void PreorderTrave() {Preorder(root);};
private:
	void Inorder(BeachNode *subTree);
	void Preorder(BeachNode *subTree);

	

private:

    BeachNode *root;
};

