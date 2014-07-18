#pragma once
#include "DCEL.h"
#include "BeachLine.h"
#include "avl.hpp"

class Voronoi
{
public:
	Voronoi(void);
	~Voronoi(void);

	void InitEventQueue(std::vector<Point> &pointList);
	void VoronoiDiagram(std::vector<Point> &pointList);
	void HandleSiteEvent();
	void HandleCircleEvent();
    std::vector<Line> GetEdges();
private:

	void FindLeftCircleEvent(BeachNode *leftArc);
	void FindRightCircleEvent(BeachNode *leftArc);
	Point* FindCircle(Point &left,Point &middle,Point &right);
	bool IsRightOfLine(Point &start, Point &end, Point &point);
	Point* ComputeCircleCenterDefinedBy3Points(Point &p1,Point &p2,Point &p3);
	BeachNode* BuildSubBeachLine (Point &leftArcSite, Point &rightArcSite);
	void DeleteArc(BeachNode *delArc);
	void CheckCircleEvent(BeachNode *arc);
	void HandleBox(BeachNode *pNode);
	float ComputeX(float y, Point &leftPoint, Point &rightPoint);
	float ComputeY(float x, Point &leftPoint, Point &rightPoint);
	std::vector<Point> ComputeEndPoint (Point leftSite, Point rightSite);
private:
	EventNode *curEvent;
	DCEL dcel;
	avl<EventNode> eventQueue;
	BeachLine beachLine;
};

