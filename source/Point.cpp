#include "Point.h"

bool FEqual(float f1, float f2) 
{
	return (abs(f1 - f2) < 1e-4f);
}

float Point::operator^(const Point &p)
{
	return (x_ * p.y_ - y_ * p.x_);
}

bool Point::operator>(const Point &p)
{
	return (x_ > p.x_ || (FEqual(x_, p.x_) && y_ > p.y_) );
}

bool Point::operator<(const Point &p)
{
	return (x_ < p.x_ || (FEqual(x_ , p.x_) && y_ < p.y_) );
}
	
bool Point::operator==(const Point &p)
{
	return ( FEqual(x_ , p.x_) && FEqual(y_ , p.y_));
}

float Point::Dist(Point &p)
{
	float px = p.x_;
	float py = p.y_;

	return sqrt(pow((x_-px),2)+pow((y_-py),2));
}

bool Point::Cocircle(Point p1,Point p2,Point p3,Point &center)
{
  float x12=p2.x_-p1.x_;
  float y12=p2.y_-p1.y_;
  float x13=p3.x_-p1.x_;
  float y13=p3.y_-p1.y_;
  float z2=x12*(p1.x_+p2.x_)+y12*(p1.y_+p2.y_);
  float z3=x13*(p1.x_+p3.x_)+y13*(p1.y_+p3.y_);
  float d=2.0*(x12*(p3.y_-p2.y_)-y12*(p3.x_-p2.x_));
  //if(abs(d)<EP)                           //共线，圆不存在
   //  return false;
  center.x_=(y13*z2-y12*z3)/d;
  center.y_=(x12*z3-x13*z2)/d;
 
  return true;
}

bool Point::IsRightOfLine(Point start, Point end, Point point) 
{
    return (end.x_ - start.x_) * (point.y_ - start.y_) - (end.y_ - start.y_) * (point.x_ - start.x_) <= 0;
}

bool Point::SameLineThrough3Point(Point &start,Point &end)
{
	if ((end.GetX() - start.GetX()) * (y_ - start.GetY()) - (end.GetY() - start.GetY()) * (x_ - start.GetX()) == 0)
		return true;
	else
		return false;
}

bool Point::Higher(const Point &p)
{
	return (y_ > p.y_ || (FEqual(y_ , p.y_) && x_ < p.x_) );
}