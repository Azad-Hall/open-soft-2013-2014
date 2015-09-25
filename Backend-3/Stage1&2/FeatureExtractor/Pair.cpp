#include "pair.h"
#include <math.h>

Pair::Pair(int x_,int y_):
x(x_),
y(y_)
{}
Pair Pair::operator+(const Pair& b)
{
	return Pair(this->x+b.x,this->y+b.y);
}
Pair Pair::operator-(const Pair& b)
{
	return Pair(this->x-b.x,this->y-b.y);
}
Pair Pair::operator*(float b)
{
	return Pair(this->x*b,this->y*b);
}
bool Pair::operator==(const Pair& b)
{
	return (this->x==b.x && this->y==b.y);
}

float Pair::dot(Pair q)     
{ 
	return x*q.x+y*q.y; 
}

float Pair::cross(Pair q)
{
	return x*q.y - y*q.x;
}

Pair Pair::disp(Pair q)   
{ 
	return (*this-q); 
}

float Pair::length2()
{
	return this->dot(*this);
}

Pair Pair::projectOnSegment(Pair a,Pair b) 
{
	float edgeLength2 = (b-a).dot(b-a);
	float projectionLengthNorm = ((*this)-a).dot(b-a)/edgeLength2;
	if(projectionLengthNorm < 0)
		return a;
	if(projectionLengthNorm > 1)
		return b;
	return a + (b-a) * projectionLengthNorm;
}
