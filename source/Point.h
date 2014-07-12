#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>

class Point
{
public:
	Point():x_(0),y_(0) {};
	Point(float x, float y) : x_(x), y_(y) {};
	Point(const Point& p) { x_ = p.x_; y_ = p.y_;};
	Point & operator=(const Point& p) { x_=p.x_; y_=p.y_; return *this;}   
	float GetX() {return x_;};
	float GetY() {return y_;};
	void SetX(float x) {x_ = x;};
	void SetY(float y) {y_ = y;};
	float operator^(const Point &p);
	bool operator>(const Point &p);
	bool operator<(const Point &p);
	bool operator==(const Point &p);
	float Dist(Point &p);
	bool Cocircle(Point p1,Point p2,Point p3,Point &center);
	bool IsRightOfLine(Point start, Point end, Point point); 
	void PrintCoordinate() {std::cout << "  ("<<x_<<","<<y_<<")  ";};
	bool SameLineThrough3Point(Point &start,Point &end);
	bool Higher(const Point &p);
	~Point(void){};
private:
	float x_;
	float y_;
};

