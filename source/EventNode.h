#pragma once
#include "Point.h"
class BeachNode;
enum EventType {SITE, CIRCLE};
class EventNode
{
public:
	EventNode(Point p,EventType t,BeachNode *a,Point c) : event(p),type(t),arc(a),height(0),left(NULL),right(NULL),
		parent(NULL),center(c){};
	EventNode(Point p,EventType t,BeachNode *a) : event(p),type(t),arc(a),height(0),left(NULL),right(NULL),
		parent(NULL){};
	int GetHeight()       { return height; }
	void SetHeight(int h) { height = h;    }
	Point GetEvent()      { return event;  }
	bool Prior(EventNode*);
	void Print()          { event.PrintCoordinate(); }
	void SwapKey(EventNode *e);
	EventType GetEventType() { return type; }
	BeachNode* GetArc();
	Point GetCenter() { return center;}
	~EventNode(void){};
public:
	EventNode *left;
	EventNode *right;
	EventNode *parent;
private:
	Point event;
	Point center;
	EventType type;
	BeachNode *arc;
	int height;
};

