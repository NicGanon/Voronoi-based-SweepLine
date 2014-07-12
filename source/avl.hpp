#include "EventNode.h"
#include "BeachNode.h"

template<typename T>
class avl
{
public:
	avl():root(NULL){};
	~avl(){};

	void LeftRotation(T* subTree);
	void RightRotation(T* subTree);
	void Insert(T *newNode);
	void InsertFixup(T *newNode);
	int GetHeight(T *subTree);
	void DeleteFixup(T *fixNode);
    void Delete(T *delNode);
	T* MinNode(T *subTree);
	T* Successor(T *subTree);
	T* CurEvent(){return MinNode(root);};
	bool Empty();
	void InorderTrave() {Inorder(root);};
	void PreorderTrave() {Preorder(root);};
private:
	void Inorder(T *subTree);
	void Preorder(T *subTree);

    T *root;
};

template<typename T>
bool avl<T>::Empty()
{
	if (root == NULL)
		return true;
	else
		return false;
}

template<typename T>
int avl<T>::GetHeight(T *subTree)
{
	if (subTree == NULL)
		return 0;
	else 
		return subTree->GetHeight();
}

template<typename T>
void avl<T>::LeftRotation(T* subTree)
{
	T *rchild = subTree->right;
	
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

template<typename T>
void avl<T>::RightRotation(T* subTree)
{
	T *lchild = subTree->left;
	
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


template<typename T>
void avl<T>::Insert(T *newNode)
{
	//cout << "insert: "<<key<<endl;
	T *pNode = root;
	T *parent = NULL;

	while (pNode != NULL) {
	    parent = pNode;
		if ( newNode->Prior(pNode) ) 
			pNode = pNode->left;
		else 
		    pNode = pNode->right;
	}
	
	if (parent == NULL) 
		root = newNode;
    else if (newNode->Prior(parent) )
		parent->left = newNode;
	else
		parent->right = newNode;

	newNode->parent = parent;
    
	InsertFixup(newNode);
}

template<typename T>
void avl<T>::InsertFixup(T *newNode)
{
	newNode->SetHeight(std::max(GetHeight(newNode->left),GetHeight(newNode->right))+1);
	T *fixNode = newNode->parent;
	
	while (fixNode) {
	    
		if(GetHeight(fixNode->left) - GetHeight(fixNode->right) == 2) {
		    if (fixNode->left->Prior(newNode))
				LeftRotation(fixNode->left);
			
		    RightRotation(fixNode);
			fixNode = fixNode->parent;
		}
		else if(GetHeight(fixNode->right) - GetHeight(fixNode->left) == 2) {
		    if (newNode->Prior(fixNode->right))
				 RightRotation(fixNode->right);
				
			LeftRotation(fixNode);
			fixNode = fixNode->parent;
		}
		else
		    fixNode->SetHeight(std::max(GetHeight(fixNode->left),GetHeight(fixNode->right))+1);
		fixNode = fixNode->parent;
	    
	}
}

template<typename T>
void avl<T>::Inorder(T *subTree)
{
	if (subTree) {
		Inorder(subTree->left);
		subTree->Print();
		Inorder(subTree->right);
	}
}

template<typename T>
void avl<T>::Preorder(T *subTree)
{
	if (subTree) {
		subTree->Print();
		Preorder(subTree->left);
		Preorder(subTree->right);
	}
}



template<typename T>
T* avl<T>::MinNode(T *subTree)
{
	while (subTree->left)
		subTree = subTree->left;
	return subTree;
}

template<typename T>
T* avl<T>::Successor(T *subTree)
{
	if (subTree->right != NULL)
	    return MinNode(subTree->right);
	while(subTree == subTree->parent->right)
		subTree = subTree->parent;

	return subTree;
}

template<typename T>
void avl<T>::Delete(T *delNode)
{
	if (delNode == NULL) {
		return;
	}
	T *realDelNode = NULL;
	
	if (delNode->left == NULL || delNode->right == NULL)
		realDelNode = delNode;
	else
		realDelNode = Successor(delNode);
	
	  T *son = (realDelNode->left != NULL) ? realDelNode->left : realDelNode->right;
	if (son)
		son->parent = realDelNode->parent;
	
	if (realDelNode->parent == NULL)
		root = son;
    else if (realDelNode == realDelNode->parent->left)
		realDelNode->parent->left = son;
	else
		realDelNode->parent->right = son;
												
	if (delNode != realDelNode)
		delNode->SwapKey(realDelNode);
	
	
	DeleteFixup(realDelNode->parent);//fix from the parent of realdelNode
	delete(realDelNode);	
}

template<typename T>
void avl<T>::DeleteFixup(T *fixNode)
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