#include "BeachLine.h"


bool BeachLine::Empty()
{
	if (root == NULL)
		return true;
	else
		return false;
}

int BeachLine::GetHeight(BeachNode *subTree)
{
	if (subTree == NULL)
		return 0;
	else 
		return subTree->GetHeight();
}


void BeachLine::LeftRotation(BeachNode* subTree)
{
	BeachNode *rchild = subTree->right;
	
	subTree->right = rchild->left;
	if (rchild->left)
		rchild->left->parent = subTree;

	rchild->parent = subTree->parent;
	 
	if (subTree->parent == NULL) 
		root = rchild;
	else if (subTree == subTree->parent->left)
		subTree->parent->left = rchild;
	else
		subTree->parent->right = rchild;
	rchild->left = subTree;
	subTree->parent = rchild;

	subTree->SetHeight(std::max(GetHeight(subTree->left),GetHeight(subTree->right))+1);
	rchild->SetHeight(std::max(GetHeight(rchild->left),GetHeight(rchild->right))+1);
	
}


void BeachLine::RightRotation(BeachNode* subTree)
{
	BeachNode *lchild = subTree->left;
	
	subTree->left = lchild->right;
	if (lchild->right)
		lchild->right->parent = subTree;
	
	lchild->parent = subTree->parent;
	if (subTree->parent == NULL) 
		root = lchild;
	else if ( subTree == subTree->parent->left)
		    subTree->parent->left = lchild;
	else
		    subTree->parent->right = lchild;    
	lchild->right = subTree;
	subTree->parent = lchild;
    
	subTree->SetHeight(std::max(GetHeight(subTree->left),GetHeight(subTree->right))+1);
	lchild->SetHeight(std::max(GetHeight(lchild->left),GetHeight(lchild->right))+1);

}


void BeachLine::Insert(BeachNode*pos,BeachNode* subTree)
{
	BeachNode *parent = pos->parent;
	if (parent == NULL) 
		root = subTree;
	else if (pos == pos->parent->left )
		parent->left = subTree;
	else
		parent->right = subTree;

	subTree->parent = parent;
    
	if (pos->suc) {  //link the right leaf of sub tree to the suc of the leaf which was replaced;
	    BeachNode *rightLeaf = subTree;
	    while(rightLeaf->GetType() != ARC)
		    rightLeaf = rightLeaf->right;
	    rightLeaf->suc = pos->suc;
	    pos->suc->pre = rightLeaf;
	}
	InsertFixup(subTree);
}


void BeachLine::InsertFixup(BeachNode *newNode)
{
	//newNode->SetHeight(std::max(GetHeight(newNode->left),GetHeight(newNode->right))+1);
	BeachNode *fixNode = newNode->parent;
	
	while (fixNode) {
	    
		if(GetHeight(fixNode->left) - GetHeight(fixNode->right) == 2) {
			if (GetHeight(fixNode->left->right) > GetHeight(fixNode->left->left))
				LeftRotation(fixNode->left);
			
		    RightRotation(fixNode);
			fixNode = fixNode->parent;
		}
		else if(GetHeight(fixNode->right) - GetHeight(fixNode->left) == 2) {
		    if (GetHeight(fixNode->right->left) > GetHeight(fixNode->right->right))
				 RightRotation(fixNode->right);
				
			LeftRotation(fixNode);
			fixNode = fixNode->parent;
		}
		else
		    fixNode->SetHeight(std::max(GetHeight(fixNode->left),GetHeight(fixNode->right))+1);
		fixNode = fixNode->parent;
	    
	}
}

BeachNode* BeachLine::FindArc(Point &site)
{
	BeachNode *pNode = root;
	while (pNode) {
		if (pNode->GetType() == ARC)
			break;
		else {
			if (site.Dist(pNode->GetLeftArc()) < site.Dist(pNode->GetRightArc()) )
				pNode = pNode->left;
			else
			    pNode = pNode->right;
		}
	}
	
	return pNode;
}
	
BeachNode* BeachLine::BuildSubTree(BeachNode *arc , Point &site)
{
	BeachNode *subTree = nullptr;
	if (arc == nullptr)
		subTree = new BeachNode(ARC,site,nullptr,nullptr,nullptr);
	else {
	    HalfEdge *newEdge = new HalfEdge();
	    BeachNode *leftBreakPoint = new BeachNode(BKPOINT,arc->GetLeftArc(),site,newEdge);
	    BeachNode *rightBreakPoint = new BeachNode(BKPOINT,site,arc->GetLeftArc(),newEdge);
		
		BeachNode *leftHalfArc = new BeachNode(ARC,arc->GetLeftArc(),nullptr,nullptr,nullptr);
	    BeachNode *rightHalfArc = new BeachNode(ARC,arc->GetLeftArc(),nullptr,nullptr,nullptr);
		BeachNode *newArc = new BeachNode(ARC,site,nullptr,leftHalfArc,rightHalfArc);

		leftHalfArc->pre  = arc->pre; leftHalfArc->suc  = newArc;
		rightHalfArc->pre = newArc;   rightHalfArc->suc = arc->suc;

	    leftBreakPoint->left = leftHalfArc;
	    leftHalfArc->parent = leftBreakPoint;
	    leftBreakPoint->right = rightBreakPoint;
	    rightBreakPoint->parent = leftBreakPoint;
	    rightBreakPoint->left = newArc;
	    newArc->parent = rightBreakPoint;
	    rightBreakPoint->right = rightHalfArc;
	    rightHalfArc->parent = rightBreakPoint;

	    newArc->UpdateHeight();
	    leftHalfArc->UpdateHeight();
	    rightHalfArc->UpdateHeight();
	    rightBreakPoint->UpdateHeight();
	    leftBreakPoint->UpdateHeight();

		subTree = leftBreakPoint;
	}

	return subTree;
}

BeachNode* BeachLine::BuildSubBeachLine (Point &leftArcSite, Point &rightArcSite)
{
	HalfEdge *origin = new HalfEdge();
	BeachNode *newBreakPoint = new BeachNode(BKPOINT,leftArcSite,rightArcSite,origin);
	
	BeachNode *lchild = new BeachNode(ARC,leftArcSite,nullptr,nullptr,nullptr);
	newBreakPoint->left = lchild;
	lchild->parent = newBreakPoint;

	BeachNode *rchild = nullptr;
	if (leftArcSite.Higher(rightArcSite)) {
		rchild = BuildSubBeachLine(rightArcSite,leftArcSite);
	}
	else
		rchild = new BeachNode(ARC,rightArcSite,nullptr,nullptr,nullptr);
	newBreakPoint->right = rchild;
	rchild->parent = newBreakPoint;

	return newBreakPoint;
}

void BeachLine::Inorder(BeachNode *subTree)
{
	if (subTree) {
		Inorder(subTree->left);
		//subTree->Print();
		Inorder(subTree->right);
	}
}


void BeachLine::Preorder(BeachNode *subTree)
{
	if (subTree) {
		//subTree->Print();
		Preorder(subTree->left);
		Preorder(subTree->right);
	}
}

void BeachLine::DeleteFixup(BeachNode *fixNode)
{
	while (fixNode) {
		if (GetHeight(fixNode->left) - GetHeight(fixNode->right) == 2) {
			if (GetHeight(fixNode->left->right) > GetHeight(fixNode->left->left))
				LeftRotation(fixNode->left);
			RightRotation(fixNode);
		}
		else if (GetHeight(fixNode->right) - GetHeight(fixNode->left) == 2) {
		    if (GetHeight(fixNode->right->left) > GetHeight(fixNode->right->right))
				RightRotation(fixNode->right);
			LeftRotation(fixNode);
		}
		else
			fixNode->SetHeight(std::max(GetHeight(fixNode->left), GetHeight(fixNode->right))+1);
		fixNode = fixNode->parent;
	}
}
