#ifndef __POLYGON_H_INCLUDED__
#define __POLYGON_H_INCLUDED__

//forward declared dependencies

//included dependencies
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "pair.h"
#include "../Stage1/Utility.h"
#include "../Stage1/Image.h"
#include "../Stage1/imageFeatures.h"
#include "../Stage1/imageProcessing.h"
#include "../Stage1/morphologyConstants.h"

class MatchFeatures
{
public:
	Pair startindex,endindex;
	Pair initIndex;
	Pair translation;
	float rotation;
	int matchcount;
	float matchlength;
	Pair ctranslation;
	float crotation;
};

class Polygon
{
public:
	int nVertices;
	Pair* vertex;
	float* angle;
	float* edgeLength;
	bool order;
	Image* image;
	Image* mask;
	Image* maskedImage;
	Image* entropyImage;

	Polygon(char *filename);
	Polygon(Link* vertices);
	Polygon(Polygon *p);
	float temp;

	bool is_inside(Pair point)
	{
		int count = 0, i = 0;
		do
		{
			int next = (i+1)%nVertices;
 
			if(abs(point.y - vertex[next].y) + abs(vertex[i].y - point.y) <= abs(vertex[i].y - vertex[next].y))
			{
				
				temp = (abs(point.y - vertex[i].y)/((float)abs(vertex[next].y - vertex[i].y))) * (vertex[next].x - vertex[i].x) + vertex[i].x;
				if(temp > point.x)
				{
        			count++;
				}
			}
        	
			i = next;
		} while (i != 0);

		return count&1;  // Same as (count%2 == 1)
	}


	~Polygon();
	void updateGeometry();
	Polygon* reorientTo(Polygon* reference, int Index, int refIndex,Pair &trans,float &rotation);
};

#endif
