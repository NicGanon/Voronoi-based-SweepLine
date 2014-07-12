#include "DCEL.h"

Face* DCEL::FindFace(Point &site)
{
	auto it = Faces.begin();
	while (it != Faces.end()) {
	    if ((*it)->GetSite() == site)
			return *it;

		++it;
	}

	Face *newFace = new Face(site);
	Faces.push_back(newFace);

	return newFace;
}