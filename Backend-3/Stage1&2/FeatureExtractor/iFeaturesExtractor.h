#ifndef __IFEATURESEXTRACTOR_H_INCLUDED__
#define __IFEATURESEXTRACTOR_H_INCLUDED__

//forward declared dependencies
class Pair;
class Boundary;

//included dependencies
#include "boundaryFeatures.h"
#include "boundaryAngles.h"

class IFeaturesExtractor
{
public:
	//all polygonal features extraction functions
	/*virtual vector<double> extractAllPolygonAngles(bool isDegree) = 0;
	virtual double getInternalAngle(Pair a,bool isDegree) = 0;
	virtual double getInternalAngle(int i,bool isDegree) = 0;*/

	//all boundary features extraction functions
	/*virtual double getEucledeanDistance(Boundary b) = 0;
	virtual double getEucledeanDistance(Pair a,Pair b) = 0;
	virtual vector<BoundaryFeatures> getAllBoundariesFeatures() = 0;
	virtual void calculateAllBoundariesFeatures() = 0;
	virtual BoundaryFeatures getBoundaryFeatures(Boundary b) = 0;
	virtual BoundaryFeatures getBoundaryFeatures(Pair r,Pair l) = 0;
	virtual BoundaryAngles getBoundaryAngles(Pair r,Pair l) = 0;
	virtual BoundaryAngles getBoundaryAngles(Boundary b) = 0;
	virtual BoundaryAngles getBoundaryAngles(int edgeNum) = 0;
	virtual BoundaryFeatures getBoundaryFeatures(int edgeNum) = 0;*/
};

#endif
