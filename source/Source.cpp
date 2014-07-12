#include "Voronoi.h"

/*void UnitTest()
{
	avl<EventNode> eventQueue;
	const int N = 7;
	Point array[N] = {Point(4,4),Point(1,1),Point(2,3),Point(4,8),Point(9,7),Point(11,5),Point(10,6)};
	
	for (int i=0;i<N;++i) {
	    EventNode *newNode = new EventNode(array[i],SITE,NULL);
		eventQueue.Insert(newNode);
		
		
	}

	while (!eventQueue.Empty()) {
		EventNode *curEvent = eventQueue.CurEvent();
		eventQueue.Delete(curEvent);

		std::cout<<"inorder: ";
		eventQueue.InorderTrave();
	    std::cout<<std::endl;
	    std::cout<<"preorder: ";
		eventQueue.PreorderTrave();
		std::cout<<std::endl<<std::endl;
	}
	
}*/

void InitEvent(std::vector<Point> &eventList)
{
	const int N = 3;
	Point array[N] = {Point(30,80),Point(60,60),Point(20,40)};
	
	for (int i=0;i<N;++i) {
		eventList.push_back(array[i]);
		
		
	}
}

void UnitTest()
{
	std::vector<Point> eventList;
	InitEvent(eventList);
	Voronoi vDiagram;

	vDiagram.VoronoiDiagram(eventList);
}

int main()
{
	UnitTest();
	getchar();
}