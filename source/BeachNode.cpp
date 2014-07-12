#include "BeachNode.h"


BeachNode::BeachNode(NodeType t,Point bpl,Point bpr, HalfEdge *e): type(t),arcl(bpl),arcr(bpr),originHalfEdge(e),circleEvent(NULL),
	left(nullptr),right(nullptr),parent(nullptr),height(1),pre(nullptr),suc(nullptr)
{}
	
BeachNode::BeachNode(NodeType t,Point leaf,EventNode *circle,BeachNode *forward,BeachNode *backward) :type(t),arcl(leaf),
	circleEvent(circle),originHalfEdge(nullptr),left(nullptr),right(nullptr),parent(nullptr),height(1),pre(nullptr),suc(nullptr)
{}

BeachNode::~BeachNode(void)
{
}

void BeachNode::UpdateHeight()
{
	int leftHeight  = left  ? left->GetHeight()  : 0;
	int rightHeight = right ? right->GetHeight() : 0;

	height = std::max(leftHeight,rightHeight)+1;
}