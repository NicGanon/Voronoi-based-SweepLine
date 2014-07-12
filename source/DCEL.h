#pragma once
#include "Point.h"


class Face;
class Line 
{
public:
	Point start;
	Point end;
	Line(Point s, Point e):start(s),end(e){}
	void Print() {start.PrintCoordinate(); end.PrintCoordinate();}
};

class HalfEdge
{
private:
	Point		Start;
	
	HalfEdge	*Next;
	HalfEdge	*Prev;
	HalfEdge	*Twin;
	Face*		IncidentFace;

public:
	HalfEdge() : Next(nullptr),Prev(nullptr),Twin(nullptr),IncidentFace(nullptr) {}
	HalfEdge(Point start):Start(start),Next(nullptr),Prev(nullptr),Twin(nullptr),IncidentFace(nullptr)	{}
	
	void setIncidentFace(Face* f)		{ IncidentFace = f;			}
	void setNext(HalfEdge* next)		{ Next = next;				}
	void setPrevious(HalfEdge* prev)	{ Prev = prev;				}
	void setTwin(HalfEdge* twin)		{ Twin = twin;				}
	void setStart(Point s)				{  Start.SetX(s.GetX()); Start.SetY(s.GetY()); }


	Face*		getIncidentFace()	const { return IncidentFace;	}
	HalfEdge*	getNext()			const { return Next;			}
	HalfEdge*	getPrevious()		const { return Prev;			}
	HalfEdge*	getTwin()			const { return Twin;			}
	Line		getEdge()			const { return Line(Start,Twin->getStart());	}
	Point		getStart()			const { return Start;			}
	
};
class Face
{
private:
	HalfEdge*	IncidentEdge;
	Point		Site;
	std::vector<HalfEdge*> halfEdges;

	Face(){}
public:
	Face(Point site)						{ Site = site; }
	Face(Point site, HalfEdge incidentEdge)	{ Site = site, IncidentEdge = &incidentEdge; }
	void setIndicidentEdge(HalfEdge* e)		{ IncidentEdge = e; }
	void AddHalfEdge(HalfEdge *he)          { halfEdges.push_back(he); }
	Point		GetSite()			const	{ return Site; }
	HalfEdge*	getIncidentEdge()	const	{ return IncidentEdge; }
	std::vector<HalfEdge*>& GetHalfEdges()  { return halfEdges;    }
};
class DCEL
{
	std::vector<Face*> Faces;
public:
	unsigned getNumberOfFaces()const {return Faces.size(); }
	//std::vector<HalfEdge*> HalfEdges;
	void	addFace(Face *f)			{ Faces.push_back(f);}
	Face*	getFace(int index)	const	{ return Faces[index];}
	int getSize()				const	{ return Faces.size();}
	Face* FindFace(Point &site);
};