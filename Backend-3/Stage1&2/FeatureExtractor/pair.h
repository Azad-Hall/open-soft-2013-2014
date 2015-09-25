#ifndef __PAIR_H_INCLUDED__
#define __PAIR_H_INCLUDED__

//forward declared dependencies

//included dependencies
#define EPS 1e-12
class Pair
{
public:
	int x;
	int y;
	Pair(int x_ = 0,int y_ = 0);
	Pair operator+(const Pair& b);
	Pair operator-(const Pair& b);
	Pair operator*(float b);
	bool operator==(const Pair& b);
	float dot(Pair q);
	float cross(Pair q);
	Pair disp(Pair q);
	float length2();
	Pair projectOnSegment(Pair b, Pair c);
};

#endif
