#ifndef __POLYGONFEATURESEXTRACTOR_H_INCLUDED__
#define __POLYGONFEATURESEXTRACTOR_H_INCLUDED__

//forward declared dependencies
class Polygon;

//included dependencies
#include <vector>
#include "featuresExtractor.h"
#include "pair.h"

using namespace std;

class PolygonFeaturesExtractor: public FeaturesExtractor
{
private:
	vector<double> internalAngles;
	vector<Pair> vertices;
	double* hashAngles;   //assumed size is 1000000.
	int maxCoordinate;
	void calcMaxCoordinate();
public:
	bool order;
	PolygonFeaturesExtractor();
	PolygonFeaturesExtractor(Polygon p_);
	vector<double> extractAllPolygonAngles(bool isDegree);
	double getInternalAngle(Pair a,bool isDegree);
	double getInternalAngle(int i,bool isDegree);
	
};

#endif
