#pragma once
#include "Point.h"
#include "DCEL.h"
enum NodeType {ARC,BKPOINT};
class EventNode;
class BeachNode
{
public:
	BeachNode(NodeType t,Point bpl,Point bpr, HalfEdge *origin);
	BeachNode(NodeType t,Point leaf,EventNode *circle,BeachNode *forward,BeachNode *backward);
	~BeachNode(void);
	int GetHeight() {return height;};
	void SetHeight(int h) {height = h;};
	bool Prior(BeachNode*);
	NodeType GetType() { return type;};
	Point GetLeftArc() { return arcl;};
	Point GetRightArc() { return arcr;};
	//void Print() {event.PrintCoordinate();};
	void SwapKey(BeachNode *e);
	void UpdateHeight();
	EventNode* GetCircleEvent() { return circleEvent; }
	void SetCircleEvent (EventNode *ce) {circleEvent = ce;}
	void SetArcl(Point p) { arcl.SetX(p.GetX()); arcl.SetY(p.GetY());}
	void SetArcr(Point p) { arcr.SetX(p.GetX()); arcr.SetY(p.GetY());}
public:
	BeachNode *left;
	BeachNode *right;
	BeachNode *parent;

	HalfEdge *originHalfEdge;

	BeachNode *pre;//for link leaf node 
	BeachNode *suc;
private:
	NodeType type;
	Point arcl;
	Point arcr;
	EventNode *circleEvent;
	int height;
};

