#ifndef __IFEATURESEXTRACTOR_H_INCLUDED__
#define __IFEATURESEXTRACTOR_H_INCLUDED__

//forward declared dependencies

//included dependencies
#include "polygon.h"

class IFeaturesExtractor
{
private:
protected:
	Polygon p;
public:
	IFeaturesExtractor();
	IFeaturesExtractor(Polygon p_);
	virtual vector<double> extractAllPolygonAngles(bool isDegree) = 0;
	virtual double getInternalAngle(Pair a,bool isDegree) = 0;
	virtual double getInternalAngle(int i,bool isDegree) = 0;
};

#endif
