#include "polygon.h"
#include <stdlib.h>

const double PI = 3.1415926535;

Polygon::Polygon(Link* vertices)
{
	Link* currentCorner = vertices;
	nVertices = 0;
	while(currentCorner != NULL)
	{
		nVertices++;
		currentCorner = currentCorner->next;
	}
	
	vertex = (Pair*)calloc(nVertices, sizeof(Pair));
	currentCorner = vertices;

	for(int i = 0; i < nVertices; i++)
	{
		vertex[i].x = currentCorner->data->x;
		vertex[i].y = currentCorner->data->y;
		currentCorner = currentCorner->next;
	}

	angle = NULL;
	edgeLength = NULL;
}

Polygon::Polygon(Polygon *p)
{
	nVertices = p->nVertices;
	vertex = (Pair*)calloc(nVertices, sizeof(Pair));
	angle = (float*)calloc(nVertices, sizeof(float));
	edgeLength = (float*)calloc(nVertices, sizeof(float));

	for(int i = 0; i <  nVertices; i++)
	{
		vertex[i].x = p->vertex[i].x;
		vertex[i].y = p->vertex[i].y;
		angle[i] = p->angle[i];
		edgeLength[i] = p->edgeLength[i];
	}

	order = p->order;
}

Polygon::~Polygon()
{
	if(vertex)
	{
		free(vertex);
	}
	if(angle)
	{
		free(angle);
	}
	if(edgeLength)
	{
		free(edgeLength);
	}
}

void Polygon::updateGeometry()
{
	angle = (float*)calloc(nVertices, sizeof(float));
	edgeLength = (float*)calloc(nVertices, sizeof(float));
	
	//locals
	float angleSum = 0;
	int isReflex;
	int dotProduct;
	float magnRightSqr;
	float magnLeftSqr;
	float cosOfAngleSqr;
	float cosOfAngle;
	Pair leftVector(0,0);
	Pair rightVector(0,0);

	for(int i = 0; i < nVertices; i++)
	{
		Pair leftVector = vertex[(nVertices-1+i) % nVertices] - vertex[i];
		Pair rightVector = vertex[(i+1) % nVertices] - vertex[i];

		isReflex = (rightVector.x*leftVector.y-rightVector.y*leftVector.x);
		dotProduct = (rightVector.x*leftVector.x + rightVector.y*leftVector.y);

		magnRightSqr = (rightVector.x*rightVector.x) + (rightVector.y*rightVector.y);
		magnLeftSqr = (leftVector.x*leftVector.x) + (leftVector.y*leftVector.y); 

		cosOfAngleSqr = 0;

		if(magnLeftSqr==0 || magnRightSqr ==0)
		{
			cosOfAngle = 1;
		}
		else
		{
			cosOfAngle = dotProduct/sqrt(magnLeftSqr*magnRightSqr);
		}

		angle[i] = acos(cosOfAngle);
		/*
		if(dotProduct<0)
		{
			angle[i] = PI - acos(cosOfAngle);
		}
		else
		{
			angle[i] = acos(cosOfAngle);
		}
		*/
		if(isReflex<0)
		{
			angle[i] = 2*PI-angle[i];
		}

		angleSum += angle[i];
		edgeLength[i] = sqrt((float)magnRightSqr);
	}

	if(angleSum >= PI*nVertices)
	{
		order = true;
		for(int i=0;i<nVertices;i++)
		{
			angle[i] = 360 - angle[i]*180/PI;
		}
	}
	else
	{
		order = false;
		for(int i=0;i<nVertices;i++)
		{
			angle[i] = angle[i]*180/PI;
		}
	}
}

Polygon* Polygon::reorientTo(Polygon* reference, int index, int refIndex,Pair &ctrans,float &crotation)
{

	Polygon* output = new Polygon(this);

	//determine translation
	Pair shift(reference->vertex[refIndex].x - vertex[index].x, reference->vertex[refIndex].y - vertex[index].y);
	ctrans = shift;
	//translate
	for(int i = 0; i < nVertices; i++)
	{
		output->vertex[i].x = output->vertex[i].x + shift.x;
		output->vertex[i].y = output->vertex[i].y + shift.y;
	}

	
	//determine rotation
	int isReflex;
	int magRefSqr;
	int magOutSqr;
	float dotProd;
	float rotation;
	if(output->order == reference->order)
	{
		Pair refEdge(reference->vertex[(refIndex+1) % reference->nVertices].x - reference->vertex[refIndex].x, reference->vertex[(refIndex+1) % reference->nVertices].y - reference->vertex[refIndex].y);
		Pair outEdge(output->vertex[(index-1+output->nVertices)%output->nVertices].x - output->vertex[index].x, output->vertex[(index-1+output->nVertices)%output->nVertices].y - output->vertex[index].y);
		isReflex = outEdge.x * refEdge.y - outEdge.y * refEdge.x;
		magRefSqr = (refEdge.x * refEdge.x) + (refEdge.y * refEdge.y);
		magOutSqr = (outEdge.x * outEdge.x) + (outEdge.y * outEdge.y); 
		dotProd = (refEdge.x * outEdge.x) + (refEdge.y * outEdge.y);
		rotation = acos(dotProd/(sqrt((float)magRefSqr) * sqrt((float)magOutSqr)));
		if(isReflex > 0)
		{
			rotation = 2*PI - rotation;
		}
	}
	else
	{
		Pair refEdge(reference->vertex[(refIndex+1) % reference->nVertices].x - reference->vertex[refIndex].x, reference->vertex[(refIndex+1) % reference->nVertices].y - reference->vertex[refIndex].y);
		Pair outEdge(output->vertex[(index+1) % output->nVertices].x - output->vertex[index].x, output->vertex[(index+1) % output->nVertices].y - output->vertex[index].y);
		isReflex = outEdge.x * refEdge.y - outEdge.y * refEdge.x;
		magRefSqr = (refEdge.x * refEdge.x) + (refEdge.y * refEdge.y);
		magOutSqr = (outEdge.x * outEdge.x) + (outEdge.y * outEdge.y); 
		dotProd = (refEdge.x * outEdge.x) + (refEdge.y * outEdge.y);
		rotation = acos(dotProd/(sqrt((float)magRefSqr) * sqrt((float)magOutSqr)));
		if(isReflex > 0)
		{
			rotation = 2*PI - rotation;
		}
	}
	crotation = rotation;
	//rotate
	Pair rot(0,0);
	for(int i = 0; i < nVertices; i++)
	{
		shift.x = output->vertex[i].x - reference->vertex[refIndex].x;
		shift.y = output->vertex[i].y - reference->vertex[refIndex].y;
		rot.x = shift.x * cos(rotation) + shift.y * sin(rotation);
		rot.y = -shift.x * sin(rotation) + shift.y * cos(rotation);
		output->vertex[i].x = rot.x + reference->vertex[refIndex].x;
		output->vertex[i].y = rot.y + reference->vertex[refIndex].y;
	}
	/*
	else
	{
		//find the mean of the reference polygon
		Pair refMean(reference->vertex[refIndex].x, reference->vertex[refIndex].y);
		int count = 1;
		for(int id = (refIndex+1) % reference->nVertices; id <= refStopIndex; id = (id+1) % reference->nVertices)
		{
			refMean.x += reference->vertex[id].x;
			refMean.y += reference->vertex[id].y;
			count++;
		}
		refMean.x /= count;
		refMean.y /= count;

		//find the mean of this polygon
		Pair thisMean(vertex[index].x, vertex[index].y);
		count = 1;
		if(order != reference->order)
		{
			for(int id = (index+1) % nVertices; id <= stopIndex; id = (id+1) % nVertices)
			{
				thisMean.x += vertex[id].x;
				thisMean.y += vertex[id].y;
				count++;
			}
			thisMean.x /= count;
			thisMean.y /= count;
		}
		else
		{
			for(int id = (index - 1 + nVertices) % nVertices; id >= stopIndex; id = (id - 1 + nVertices) % nVertices)
			{
				thisMean.x += vertex[id].x;
				thisMean.y += vertex[id].y;
				count++;
			}
			thisMean.x /= count;
			thisMean.y /= count;
		}

		//determine translation
		Pair shift(refMean.x - thisMean.x, refMean.y - thisMean.y);
		//translate
		for(int i = 0; i < nVertices; i++)
		{
			output->vertex[i].x = output->vertex[i].x + shift.x;
			output->vertex[i].y = output->vertex[i].y + shift.y;
		}
	}
	*/
	return output;
}

Polygon::Polygon(char* filename)
{
	printf("Analyzing %s\n", filename);
	//Temporary image pointer
	Image* tempImage;
	//OpenCV image matrix
	Mat imageCV;
	//read the file
	imageCV = imread(filename,0);
	//check for existance of image
	if(!imageCV.dims)
	{
		//printf("Unable to load iamge for %s\n", filename);
		image = NULL;
		return;
	}
	//Convert to local class
	image = new Image(imageCV);
	//Generate Entropy image
	printf("\tGenerate Entropy image ... \n");
	entropyImage = ImageFeatures::Entropy(image, 9);
	//Image processing class
	ImageProcessing ip = ImageProcessing(entropyImage);
	//Threshold and open the image
	mask = ip.doMorphologicalOperation(MORPHO_OPEN,20);
	//extract largest region
	tempImage = ip.label(mask);
	delete mask;
	mask = tempImage;
	tempImage = mask->zeropad();
	delete mask;
	mask = tempImage;
	tempImage = image->zeropad();
	delete image;
	image = tempImage;
	//refine the mask

	printf("\tRefining the mask ... \n");
	maskedImage = ip.RefineForeground(image, &mask);

	//Approxiame image as mask

	printf("\tApproximate image as mask ... \n");
	Link* polygonCorners = ip.polyApprox(mask, 20);

	//fill the vertices
	Link* currentCorner = polygonCorners;
	nVertices = 0;
	while(currentCorner != NULL)
	{
		nVertices++;
		currentCorner = currentCorner->next;
	}
	
	vertex = (Pair*)calloc(nVertices, sizeof(Pair));
	currentCorner = polygonCorners;

	for(int i = 0; i < nVertices; i++)
	{
		vertex[i].x = currentCorner->data->x;
		vertex[i].y = currentCorner->data->y;
		currentCorner = currentCorner->next;
	}

	angle = NULL;
	edgeLength = NULL;

	//compute geometric features
	updateGeometry();
}
