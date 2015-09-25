#include <math.h>
#include "constants.h"
#include "boundaryFeaturesExtractor.h"

BoundaryFeaturesExtractor::BoundaryFeaturesExtractor(Polygon p_,Image* im_):
FeaturesExtractor(p_),
im(im_)
{
	pfe = PolygonFeaturesExtractor(p);
}

double BoundaryFeaturesExtractor::getDistance(Boundary b, bool isEuclidean)
{
	return getDistance(b.right,b.left,isEuclidean);
}

double BoundaryFeaturesExtractor::getDistance(Pair a,Pair b,bool isEuclidean)
{
	double dist = 0.0;
	if(isEuclidean)
	{
		dist = sqrt(float((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)));
	}
	else
	{
		dist = abs(a.x-b.x)+abs(a.y-b.y);
	}
	return dist;
}

BoundaryFeatures BoundaryFeaturesExtractor::getBoundaryFeatures(Boundary b)
{
	return getBoundaryFeatures(b.right,b.left);
}

vector<BoundaryFeatures> BoundaryFeaturesExtractor::getAllBoundariesFeatures()
{
	calculateAllBoundariesFeatures();
	return boundaryFeatures;
}

void BoundaryFeaturesExtractor::calculateAllBoundariesFeatures()
{
	if(boundaryFeatures.size()!=0)
		return;

	vector<Boundary> boundaries = p.getAllBoundaries();
	int size = boundaries.size();
	for(int i=0;i<size;i++)
	{
		boundaryFeatures.push_back(getBoundaryFeatures(boundaries[i]));
	}
	order = pfe.order;
}

BoundaryFeatures BoundaryFeaturesExtractor::getBoundaryFeatures(Pair r,Pair l)
{	
	BoundaryFeatures features = BoundaryFeatures();
	features.angles = getBoundaryAngles(r,l);

	features.distance = getDistance(r,l,true);

	//for pixel array
	//features.contentInfo = getBoundaryContentInfo(r,l);
	return features;
}

BoundaryAngles BoundaryFeaturesExtractor::getBoundaryAngles(Pair r,Pair l)
{
	return (BoundaryAngles(pfe.getInternalAngle(r,IS_DEGREE),pfe.getInternalAngle(l,IS_DEGREE)));
}

BoundaryAngles BoundaryFeaturesExtractor::getBoundaryAngles(Boundary b)
{
	return getBoundaryAngles(b.right,b.left);
}

BoundaryAngles BoundaryFeaturesExtractor::getBoundaryAngles(int edgeNum)
{
	if(boundaryFeatures.size()==0)
	{
		calculateAllBoundariesFeatures();
	}
	return boundaryFeatures[edgeNum].angles;
}

BoundaryContentInfo BoundaryFeaturesExtractor::getBoundaryContentInfo(Pair r,Pair l)
{
	BoundaryContentInfo bci = BoundaryContentInfo();
	//bci.pixelArray = getPixelArray(r,l);
	return bci;
}


BoundaryFeatures BoundaryFeaturesExtractor::getBoundaryFeatures(int edgeNum)
{
	if(boundaryFeatures.size()==0)
	{
		calculateAllBoundariesFeatures();
	}
	return boundaryFeatures[edgeNum];
}

/*vector<Pixel> BoundaryFeaturesExtractor::getPixelArray(Pair r,Pair l)
{
	vector<Pixel> boundaryPixelArray;
	Pair direction_vector = l-r;

	int firstDirection,i,j,numberOfPixels,newX,newY,prevX,prevY;
	float tempX,tempY,newDist,prevDist;
	newX = r.x;
	newY = r.y;
	numberOfPixels = sqrt(direction_vector.x*direction_vector.x + direction_vector.y*direction_vector.y) + 1;
	
	if(direction_vector.x >= 0)
	{
		if(direction_vector.y <= -direction_vector.x)
		{
			firstDirection = 1; //vertical
			i=-1;
			j=-1;
			//go up and left
		}
		else
		{
			if(direction_vector.y <= direction_vector.x){
				firstDirection = 0; //horizontal
				i=1;
				j=-1;
				//go right and up
			}
			else
			{
				firstDirection = 1; //vertical
				i=1;
				j=1;
				//go down and right 
			}
		}
	}
	else
	{
		if(direction_vector.y >= -direction_vector.x)
		{
			firstDirection = 1; //vertical
			i=1;
			j=1;
		}
		//go down and right
		else
		{
			if(direction_vector.y >= direction_vector.x){
				firstDirection = 0; //horizontal
				i=-1;
				j=1;
				//go left and down
			}
			else
			{
				firstDirection = 1; //vertical
				i=-1;
				j=-1;
				//go up and left
			}
		}
	}

	newDist = numberOfPixels; // setting max

	for(int iterator=1;i<numberOfPixels-1;i++)
	{
		tempX = r.x + direction_vector.x / numberOfPixels  * iterator;
		tempY = r.y + direction_vector.y / numberOfPixels  * iterator;	

		do
		{
			prevDist = newDist;
			prevX = newX;
			prevY = newY;	
			if(firstDirection == 1)
			{
				newY = prevY+j;
				newX = prevX;

				if(im.isImagePixel(newX,newY))
				{
					while(im.isImagePixel(newX,newY))
						newX+=i;
					newX-=i;
				}
				else
				{
					while(!im.isImagePixel(newX,newY))
						newX-=i;
				}
			}
			else
			{
				newY = prevY;
				newX = prevX+i;
				if(im.isImagePixel(newX,newY))
				{
					while(im.isImagePixel(newX,newY))
						newY+=j;
					newY-=j;
				}
				else
				{
					while(!im.isImagePixel(newX,newY))
						newY-=j;
				}
			}
			
			newDist = (tempX - newX)*(tempX - newX) + (tempY - newY)*(tempY - newY);
		}while(prevDist > newDist);

		boundaryPixelArray.push_back(im.getImagePixel(prevX,prevY));
	}

	return boundaryPixelArray;
}*/
