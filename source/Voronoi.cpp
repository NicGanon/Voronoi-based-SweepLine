#include "Voronoi.h"
const float MinX = 0;
const float MaxX = 100;
const float MinY = 0;
const float MaxY = 100;

Voronoi::Voronoi(void)
{
}


Voronoi::~Voronoi(void)
{
}

void Voronoi::InitEventQueue(std::vector<Point> &pointList)
{
	auto it = pointList.begin();
	while ( it != pointList.end()) {
		EventNode *newEvent = new EventNode(*it,SITE,nullptr);
		eventQueue.Insert(newEvent);
		
		++it;
	}
}

void Voronoi::VoronoiDiagram(std::vector<Point> &pointList)
{
	InitEventQueue(pointList);

	while (!eventQueue.Empty()) {
		curEvent = eventQueue.CurEvent();
		if (curEvent->GetEventType() == SITE)
			HandleSiteEvent();
		else 
			HandleCircleEvent(); 

		eventQueue.Delete(curEvent);
	}

	BeachNode *pNode = beachLine.GetRoot();
	HandleBox(pNode);

	//trave the dcel
	TraveDCEL();

}

void PrintDCEL(std::vector<Point>& sites, std::vector<Line>& edges)
{
	std::cout << "sites : " << std::endl;
	auto sit = sites.begin();
	while (sit != sites.end()) {
		sit->PrintCoordinate();
		std::cout<<std::endl;
	    ++sit;
	}
	std::cout<<"edges: " <<std::endl;
	auto eit = edges.begin();
	while (eit != edges.end()) {
		eit->Print();
		std::cout<<std::endl;
		++eit;
	}

}

void Voronoi::TraveDCEL()
{
	std::vector<Point> sites;
	std::vector<Line> edges;
	int numOfFace = dcel.getSize();
	for (int i=0; i<numOfFace; ++i) {
		Face* face = dcel.getFace(i);
		sites.push_back(face->GetSite());
		std::vector<HalfEdge*> halfEdges = face->GetHalfEdges();
		auto it = halfEdges.begin();
		while (it != halfEdges.end()) {
			Line tmp((*it)->getStart(), (*it)->getTwin()->getStart());
			edges.push_back(tmp);
		    ++it;
		}
	}

	PrintDCEL(sites,edges);
}

void Voronoi::HandleBox(BeachNode *pNode)
{	
		Point leftArcSite(pNode->GetLeftArc());
		Point rightArcSite(pNode->GetRightArc());
		Point centerPoint(pNode->originHalfEdge->getTwin()->getStart());

		std::vector<Point> endPoint;
		float x1,y1;
		x1 = ComputeX(MinY,leftArcSite,rightArcSite);
		if (! (x1 < MinX || x1 > MaxX))
			endPoint.push_back(Point(x1,MinY));

		x1 = ComputeX(MaxY,leftArcSite,rightArcSite);
		if (! (x1 < MinX || x1 > MaxX) )
			endPoint.push_back(Point(x1,MaxX));


		y1 = ComputeY(MinX,leftArcSite,rightArcSite);
		if (! (y1 < MinY || y1 > MaxY))
			endPoint.push_back(Point(MinX,y1));

		y1 = ComputeY(MaxX,leftArcSite,rightArcSite);
		if (! (y1 < MinY || y1 > MaxY))
			endPoint.push_back(Point(MaxX,y1));

		if (endPoint.size() != 2)
			std::cout << "wrong with compute intersection point" << std::endl;
		float x = (leftArcSite.GetX() + rightArcSite.GetX()) / 2;
		float y = ComputeY(x,leftArcSite,rightArcSite);
		Point midSites(x,y);
		for (auto it =endPoint.begin(); it != endPoint.end(); ++it) {
			if (it->Dist(midSites) < it->Dist(centerPoint)) {
				pNode->originHalfEdge->setStart(*it);
				break;
			}
		}
		if (pNode->left->GetType() == BKPOINT )
			HandleBox(pNode->left);
		if (pNode->right->GetType() == BKPOINT )
			HandleBox(pNode->right);
}

float Voronoi::ComputeX(float y, Point &leftPoint, Point &rightPoint)
{
	float xa = leftPoint.GetX();
	float ya = leftPoint.GetY();
	float xb = rightPoint.GetX();
	float yb = rightPoint.GetY();
	
	float x = ( 2*y*(yb-ya) + xa*xa - xb*xb + ya*ya - yb*yb ) / (2*(xa-xb));

	return x;
}

float Voronoi::ComputeY(float x, Point &leftPoint, Point &rightPoint)
{
	float xa = leftPoint.GetX();
	float ya = leftPoint.GetY();
	float xb = rightPoint.GetX();
	float yb = rightPoint.GetY();
	
	float y = ( 2*x*(xb-xa) + xa*xa - xb*xb + ya*ya - yb*yb ) / (2*(ya-yb));

	return y;
}

void Voronoi::HandleSiteEvent()
{
	Point site = curEvent->GetEvent();
	if (beachLine.Empty()) {
		BeachNode *firstArc = new BeachNode(ARC,site,nullptr,nullptr,nullptr);
		beachLine.SetRoot(firstArc);
		return;
	}

	BeachNode *arc = beachLine.FindArc(site);  // flase alarm
	if (arc->GetCircleEvent())  {
		eventQueue.Delete(arc->GetCircleEvent());
		arc->SetCircleEvent(nullptr);
	}


	BeachNode *subTree = BuildSubBeachLine(arc->GetLeftArc(),site);
	beachLine.Insert(arc,subTree); //build the subtree of current site event and insert the beachline
	
    BeachNode *newArc = beachLine.FindArc(site);
	FindLeftCircleEvent(newArc);
	FindRightCircleEvent(newArc);
}

void Voronoi::HandleCircleEvent()
{
	Point center(curEvent->GetCenter());
	
	BeachNode *arc = curEvent->GetArc();
	BeachNode *leftArc = arc->pre;
	BeachNode *rightArc = arc->suc;

	
	DeleteArc(arc);
	
	if (leftArc->GetCircleEvent()) {
		eventQueue.Delete(leftArc->GetCircleEvent());
		leftArc->SetCircleEvent(nullptr);
	}
	if (rightArc->GetCircleEvent()) {
		eventQueue.Delete(rightArc->GetCircleEvent());
		rightArc->SetCircleEvent(nullptr);
 	}

}

void Voronoi::CheckCircle(BeachNode *arc)
{
	FindLeftCircleEvent(arc);
	FindRightCircleEvent(arc);
}

void Voronoi::FindRightCircleEvent(BeachNode *rightArc)
{
	BeachNode *midArc = rightArc->pre;
	BeachNode *leftArc = midArc ? midArc->pre : nullptr;
	Point *center = nullptr;
	if (midArc && leftArc) {
		center = FindCircle(leftArc->GetLeftArc(),midArc->GetLeftArc(),rightArc->GetLeftArc());
	}

	if (center) {
		Point circlePoint(center->GetX(),center->GetY() - center->Dist(rightArc->GetLeftArc()));
		EventNode *circleEvent = new EventNode(circlePoint,CIRCLE,midArc,*center);
		midArc->SetCircleEvent(circleEvent);
		eventQueue.Insert(circleEvent);
	}
}

void Voronoi::FindLeftCircleEvent(BeachNode *leftArc)
{
	BeachNode *midArc = leftArc->suc;
	BeachNode *rightArc = midArc ? midArc->suc : nullptr;
	Point *center = nullptr;
	if (midArc && rightArc) {
		center = FindCircle(leftArc->GetLeftArc(),midArc->GetLeftArc(),rightArc->GetLeftArc());
	}

	if (center) {
		Point circlePoint(center->GetX(),center->GetY() - center->Dist(leftArc->GetLeftArc()));
		EventNode *circleEvent = new EventNode(circlePoint,CIRCLE,midArc,*center);
		midArc->SetCircleEvent(circleEvent);
		eventQueue.Insert(circleEvent);
	}
}

Point* Voronoi::FindCircle(Point &left,Point &middle,Point &right)
{
	Point* center = nullptr;
	if (left.SameLineThrough3Point(middle,right))
		return nullptr;
    center = ComputeCircleCenterDefinedBy3Points(left, middle, right);
    if (IsRightOfLine(left, middle, *center) && IsRightOfLine(middle, right, *center))
		return center;
	else
		return nullptr;
}

Point* Voronoi::ComputeCircleCenterDefinedBy3Points(Point &p1,Point &p2,Point &p3)
{
	float x12=p2.GetX()-p1.GetX();
	float y12=p2.GetY()-p1.GetY();
	float x13=p3.GetX()-p1.GetX();
	float y13=p3.GetY()-p1.GetY();
    float z2=x12*(p1.GetX()+p2.GetX())+y12*(p1.GetY()+p2.GetY());
    float z3=x13*(p1.GetX()+p3.GetX())+y13*(p1.GetY()+p3.GetY());
    float d=2.0*(x12*(p3.GetY()-p2.GetY())-y12*(p3.GetX()-p2.GetX()));
  
    Point *center = new Point((y13*z2-y12*z3)/d,(x12*z3-x13*z2)/d);
 
    return center;
}

bool Voronoi::IsRightOfLine(Point &start, Point &end, Point &point)
{
	if ((end.GetX() - start.GetX()) * (point.GetY() - start.GetY()) - (end.GetY() - start.GetY()) * (point.GetX() - start.GetX()) <= 0)
        return true;
	else
		return false;
}


BeachNode* Voronoi::BuildSubBeachLine(Point &leftArcSite, Point &rightArcSite)
{
	HalfEdge *origin = new HalfEdge();
	Face *face = dcel.FindFace(leftArcSite);
	origin->setIncidentFace(face);
	face->AddHalfEdge(origin);
	
	BeachNode *newBreakPoint = new BeachNode(BKPOINT,leftArcSite,rightArcSite,origin);
	BeachNode *lchild = new BeachNode(ARC,leftArcSite,nullptr,nullptr,nullptr);
	newBreakPoint->left = lchild;
	lchild->parent = newBreakPoint;

	BeachNode *rchild = nullptr;
	if (leftArcSite.Higher(rightArcSite)) {
		rchild = BuildSubBeachLine(rightArcSite,leftArcSite);
		origin->setTwin(rchild->originHalfEdge);
		rchild->originHalfEdge->setTwin(origin);

		lchild->suc = rchild->left;
		rchild->left->pre = lchild;
	}
	else {
		rchild = new BeachNode(ARC,rightArcSite,nullptr,nullptr,nullptr);
		lchild->suc = rchild;
		rchild->pre = lchild;
	}
	newBreakPoint->right = rchild;
	rchild->parent = newBreakPoint;

	newBreakPoint->SetHeight(std::max(newBreakPoint->left->GetHeight(),newBreakPoint->right->GetHeight())+1);
	return newBreakPoint;
}

void Voronoi::DeleteArc(BeachNode *delArc)
{
	BeachNode *leftArc  = delArc->pre;
	BeachNode *rightArc = delArc->suc;

	BeachNode *delParent = delArc->parent;
	delParent->originHalfEdge->setStart(curEvent->GetCenter());
	
	//handle brothter of the delete arc and will delete the parent of delete arc from the tree
	BeachNode *delArcBrother(nullptr);
	if (delArc ==delParent->left) {
		delArcBrother = delArc->suc;
		delArcBrother->pre = delArc->pre;
		delArc->pre->suc = delArcBrother;
	}
	else {
		delArcBrother = delArc->pre;
		delArcBrother->suc = delArc->suc;
		delArc->suc->pre = delArcBrother;
	}

	//BeachNode *delArcBrother = (delArc == delParent->left) ? delParent->right : delParent->left;
	delArcBrother->parent = delParent->parent;

	if (delParent == delParent->parent->left)
		delParent->parent->left = delArcBrother;
	else
		delParent->parent->right = delArcBrother;

	Point arcl,arcr;
	if (delArc == delParent->left) {
		arcl = delArc->pre->GetLeftArc();
		arcr = delArc->GetLeftArc();
	}
	else {
		arcl = delArc->GetLeftArc();
		arcr = delArc->suc->GetLeftArc();
	}

	BeachNode *grandParent = delArcBrother->parent;
	while (grandParent) {
		if (grandParent->GetLeftArc() == arcl && grandParent->GetRightArc() == arcr)
			break;
		grandParent = grandParent->parent;
	}
	grandParent->originHalfEdge->setStart(curEvent->GetCenter());
	
	Point leftArcSite(leftArc->GetLeftArc());
	grandParent->SetArcl(leftArcSite);
	Point rightArcSite(rightArc->GetLeftArc());
	grandParent->SetArcr(rightArcSite);

	HalfEdge *el = new HalfEdge();
	HalfEdge *er = new HalfEdge();
	
	el->setTwin(er);
	Face *face = dcel.FindFace(leftArcSite);
	el->setIncidentFace(face);
	face->AddHalfEdge(el);
	grandParent->originHalfEdge = el;

	er->setTwin(el);
	face = dcel.FindFace(rightArcSite);
	er->setIncidentFace(face);
	face->AddHalfEdge(er);
	er->setStart(curEvent->GetCenter());
	
	delete(delArc);
	delete(delParent);

	beachLine.DeleteFixup(delArcBrother->parent);//fix from the parent of realdelNode

	CheckCircle(leftArc);
	CheckCircle(rightArc);
}
