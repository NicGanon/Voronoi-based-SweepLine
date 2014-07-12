#include "EventNode.h"
#include "BeachNode.h"


void EventNode::SwapKey(EventNode *e)
{
	event = e->event;
	arc = e->arc;
	type = e->type;
}

bool EventNode::Prior(EventNode *comp)
{
	float y = event.GetY();
	float compy = comp->event.GetY();
	float x = event.GetX();
	float compx = comp->event.GetX();

	if (y > compy)
		return true;
	else if (abs(y - compy) < 1e-4f && x < compx)
		return true;
	else
		return false;
}

BeachNode* EventNode::GetArc()
{
	return arc;
}