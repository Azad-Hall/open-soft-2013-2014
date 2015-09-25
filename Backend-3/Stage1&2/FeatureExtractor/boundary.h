#ifndef __BOUNDARY_H_INCLUDED__
#define __BOUNDARY_H_INCLUDED__

//forward declared dependencies

//included dependencies
#include "pair.h"

class Boundary
{
public:
	Pair right;
	Pair left;
	Boundary(Pair right_,Pair left_);
};

#endif