#include "polygonFeaturesExtractor.h"
#include "constants.h"

#include <math.h>
const double PI = 3.1415926535;

void PolygonFeaturesExtractor::calcMaxCoordinate()
{
	if(maxCoordinate!=-1)
		return;

	int nVertices = vertices.size();
	for(int i=0;i<nVertices;i++)
	{
		if(vertices[i].x>maxCoordinate)
		{
			maxCoordinate = vertices[i].x;
		}
		if(vertices[i].y>maxCoordinate)
		{
			maxCoordinate = vertices[i].y;
		}
	}
}

PolygonFeaturesExtractor::PolygonFeaturesExtractor()
{}

PolygonFeaturesExtractor::PolygonFeaturesExtractor(Polygon p_): 
FeaturesExtractor(p_), 
maxCoordinate(-1)  //assuming all positive vertices.
{ 
	vertices = p.getAllVertices();
}

vector<double> PolygonFeaturesExtractor::extractAllPolygonAngles(bool isDegree)
{
	if(internalAngles.size()!=0)
		return internalAngles;

	int nVertices = vertices.size();
	calcMaxCoordinate();
	hashAngles = new double[maxCoordinate+maxCoordinate*maxCoordinate];

	//locals
	double angleSum = 0;
	int isReflex;
	int dotProduct;
	double magnRightSqr;
	double magnLeftSqr;
	double cosOfAngleSqr;
	double cosOfAngle;
	double* angle = (double*)calloc(nVertices, sizeof(double));

	for(int i=0;i<nVertices;i++)
	{
		Pair rightVector = vertices[(nVertices-1+i)%nVertices] - vertices[i];
		Pair leftVector = vertices[(i+1)%nVertices] - vertices[i];

		isReflex = (rightVector.x*leftVector.y-rightVector.y*leftVector.x);
		dotProduct = (rightVector.x*leftVector.x + rightVector.y*leftVector.y);

		magnRightSqr = (rightVector.x*rightVector.x)+(rightVector.y*rightVector.y);   //TODO: check if can be optimised
		magnLeftSqr = (leftVector.x*leftVector.x)+(leftVector.y*leftVector.y); 

		cosOfAngleSqr = 0;

		if(magnLeftSqr==0 || magnRightSqr ==0)
		{
			cosOfAngle = 1;
		}
		else
		{
			cosOfAngle = abs(dotProduct)/sqrt(magnLeftSqr*magnRightSqr);
		}

		if(dotProduct<0)
		{
			angle[i] = PI - acos(cosOfAngle);
		}
		else
		{
			angle[i] = acos(cosOfAngle);
		}

		if(isReflex<0)
		{
			angle[i] = 2*PI-angle[i];
		}

		angleSum += angle[i];
	}

	if(angleSum >= PI*nVertices)
	{
		order = true;
		for(int i=0;i<nVertices;i++)
		{
			angle[i] = 2*PI - angle[i];
			int key = vertices[i].x+(vertices[i].y*maxCoordinate);

			if(isDegree)
			{
				internalAngles.push_back(angle[i]*180/PI);
				hashAngles[key] = angle[i]*180/PI;
			}
			else
			{
				internalAngles.push_back(angle[i]);
				hashAngles[key] = angle[i];
			}
		}
	}
	else
	{
		order = false;
		for(int i=0;i<nVertices;i++)
		{
			int key = vertices[i].x+(vertices[i].y*maxCoordinate);

			if(isDegree)
			{
				internalAngles.push_back(angle[i]*180/PI);
				hashAngles[key] = angle[i]*180/PI;
			}
			else
			{
				internalAngles.push_back(angle[i]);
				hashAngles[key] = angle[i];
			}
		}
	}

	free(angle);
	return internalAngles;
}

double PolygonFeaturesExtractor::getInternalAngle(Pair a,bool isDegree)
{
	extractAllPolygonAngles(isDegree);
	calcMaxCoordinate();
	int key = a.x+(a.y*maxCoordinate);
	return hashAngles[key];
}

double PolygonFeaturesExtractor::getInternalAngle(int i,bool isDegree)
{
	extractAllPolygonAngles(isDegree);
	calcMaxCoordinate();
	int key = vertices[i].x+(vertices[i].y*maxCoordinate);
	return hashAngles[key];
}

