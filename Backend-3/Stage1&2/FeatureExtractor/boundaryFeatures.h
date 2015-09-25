#ifndef __BOUNDARYFEATURES_H_INCLUDED__
#define __BOUNDARYFEATURES_H_INCLUDED__

//forward declared dependencies

//included dependencies
#include "boundaryAngles.h"
#include "boundaryContentInfo.h"

class BoundaryFeatures
{
public:
	BoundaryAngles angles;
	double distance;
	BoundaryContentInfo contentInfo;
	BoundaryFeatures();
};

#endif