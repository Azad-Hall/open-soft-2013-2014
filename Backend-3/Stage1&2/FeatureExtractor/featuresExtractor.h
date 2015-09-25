#ifndef __FEATURESEXTRACTOR_H_INCLUDED__
#define __FEATURESEXTRACTOR_H_INCLUDED__

//forward declared dependencies

//included dependencies
#include "polygon.h"
#include "iFeaturesExtractor.h"

class FeaturesExtractor: public IFeaturesExtractor
{
private:
protected:
	Polygon p;
public:
	FeaturesExtractor();
	FeaturesExtractor(Polygon p_);
};

#endif