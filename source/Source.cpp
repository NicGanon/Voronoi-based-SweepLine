#include "Voronoi.h"
#include <GL/glew.h>
#include <glut.h>
#include <stdlib.h>

void InitEvent(std::vector<Point> &eventList)
{
	const int N = 4;
	Point array[N] = {Point(60,60),Point(30,80),Point(20,40),Point(80,30)};
	
	for (int i=0;i<N;++i) {
		eventList.push_back(array[i]);
		
		
	}
}

void UnitTest(std::vector<Point>& sites, std::vector<Line>& edges)
{
	std::vector<Point> eventList;
	InitEvent(eventList);
	sites = eventList;

	Voronoi vDiagram;
	vDiagram.VoronoiDiagram(eventList);
	edges = vDiagram.GetEdges();
}

void init();
void display();
std::vector<Point> sites;
std::vector<Line> edges;
int main(int argc, char**argv)
{
	UnitTest(sites,edges);
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(300,300);
	glutCreateWindow("OpenGL 3D View");
	
	init();
	glutDisplayFunc(display);
	glutMainLoop();

    return 0;
}

void init()
{
	glClearColor(0.0,0.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);
}
float height = 100;
float width  = 100;
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glPointSize(2.0);
	glBegin(GL_POINTS);
	auto it = sites.begin();
	while (it != sites.end()) {
		float x = it->GetX();
		float y = it->GetY();
		glVertex3f(x/width,y/height,0.0);
		++it;
	}
	//glVertex3f(0.3,0.8,0.0);
	//glVertex3f(0.6,0.6,0.0);
	//glVertex3f(0.2,0.4,0.0);
	glEnd();
	glBegin(GL_LINES);
	auto eit = edges.begin();
	while( eit != edges.end()) {
		glVertex3f(eit->start.GetX()/width, eit->start.GetY()/height,0.0);
		glVertex3f(eit->end.GetX()/width,   eit->end.GetY()/height,0.0);
		++eit;
	}
	/*glVertex3f(0.36,0.57,0.0);
	glVertex3f(0.65,1.0,0.0);
	glVertex3f(0.0,0.66,0.0);
	glVertex3f(0.36,0.57,0.0);
	glVertex3f(0.65,0.0,0.0);
	glVertex3f(0.36,0.57,0.0);
	*/
	glEnd();
	/*
	glBegin(GL_LINES);
	glVertex3f(0.75,0.75,0.0);
	glVertex3f(0.25,0.25,0.0);
	glEnd();*/
	glFlush();
}
