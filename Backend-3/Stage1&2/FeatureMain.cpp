#include <stdio.h>
#include <stdlib.h>

#include "../FeatureExtractor/polygon.h"

using namespace cv;
using namespace std;

//draw an array of polygons to a window
void drawPolygonArray(Polygon**p, int n, char* windowName, char* filename = NULL)
{
	//n is array length
	if(n == 0)
		return;
	//local iterator
	int i,j;
	//find bounding box
	Pair minima(p[0]->vertex[0].x, p[0]->vertex[0].y);
	Pair maxima(p[0]->vertex[0].x, p[0]->vertex[0].y);
	for(j = 0; j < n; j++)
	{
		for(i = 0; i < p[j]->nVertices; i++)
		{
			if(p[j]->vertex[i].x < minima.x)
				minima.x = p[j]->vertex[i].x;
			else if(p[j]->vertex[i].x > maxima.x)
				maxima.x = p[j]->vertex[i].x;
			if(p[j]->vertex[i].y < minima.y)
				minima.y = p[j]->vertex[i].y;
			else if(p[j]->vertex[i].y > maxima.y)
				maxima.y = p[j]->vertex[i].y;
		}
	}

	//opencv matrix
	Image im = Image(maxima.y - minima.y + 1, maxima.x - minima.x + 1, CV_8U);
	Mat image = *(im.toMat());

	int nextindex;
	for(j = 0; j < n; j++)
	{
		for(i = 0; i < p[j]->nVertices; i++)
		{
			nextindex = (i + 1)%p[j]->nVertices;
			line(image, Point(p[j]->vertex[i].x - minima.x, p[j]->vertex[i].y - minima.y), Point(p[j]->vertex[nextindex].x - minima.x, p[j]->vertex[nextindex].y - minima.y), cvScalar(255), 3);
		}
	}
	imshow(windowName, image);
	if(filename)
		imwrite(filename, image);
}

MatchFeatures edgeMatch(Polygon **p,int start0,int start1,int threshold)
{
	//reoriented polygon
	Polygon *orientedPolygon = p[1]->reorientTo(p[0],start1,start0);
	//locals
	float distFromStart0 = 0;
	float distFromStart1 = 0;
	int it0 = start0,it1 = start1;
	Pair error(0,0);
	float rotError = 0;
	int errorCount = 0;
	Pair tempPair(0,0);
	Polygon **pArray;
	pArray = (Polygon**)calloc(2, sizeof(Polygon*));
	Pair dispStart0(0,0);
	Pair dispStart1(0,0);
	Pair shift(0,0);
	Pair rot(0,0);
	Pair start(start0,start1);
	Pair end(start0,start1);
	Pair lastStart;
	Pair lastEnd;
	int matchedCorners = 0;
	bool translate = false;
	float matchlength;
	do
	{
		matchlength = 0;
		lastStart = start;
		lastEnd = end;
		it0 = start0;
		it1 = start1;
		//reset errors
		error.x = 0;
		error.y = 0;
		rotError = 0;
		errorCount = 0;
		matchedCorners = 0;
		do
		{
			if(start0 == 0 && start1 == 14)
				printf("");
			if(p[0]->order != p[1]->order)
			{
				//find disp of next points
				tempPair = p[0]->vertex[(it0+1) % p[0]->nVertices] - orientedPolygon->vertex[(it1+1) % orientedPolygon->nVertices];
				//if they are close enough move forward
				if(tempPair.length2() < threshold)
				{
					//accumalate error
					error = error + tempPair;
					errorCount++;
					matchedCorners++;
					matchlength += (orientedPolygon->vertex[it1] - orientedPolygon->vertex[(it1+1)%orientedPolygon->nVertices]).length2();

					//forward indices
					it0 = (it0+1)%p[0]->nVertices;
					it1 = (it1+1)%orientedPolygon->nVertices;

					//compute rotational error
					dispStart0 = p[0]->vertex[it0] - p[0]->vertex[start0];
					dispStart1 = orientedPolygon->vertex[it1] - orientedPolygon->vertex[start1];
					rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));
					//printf("Distance between %d of p[0] and %d of p[1] is %f\n",it0,it1, p[0]->vertex[it0].dist2(orientedPolygon->vertex[it1]));
				}
				else 
				{
					//find closer vertex
					distFromStart0 = (p[0]->vertex[(it0+1) % p[0]->nVertices] - p[0]->vertex[start0]).length2();
					distFromStart1 = (orientedPolygon->vertex[(it1+1) % orientedPolygon->nVertices] - orientedPolygon->vertex[start1]).length2();

					if(distFromStart0 < distFromStart1)
					{
						tempPair = p[0]->vertex[(it0+1)%p[0]->nVertices] - p[0]->vertex[(it0+1)%p[0]->nVertices].projectOnSegment(orientedPolygon->vertex[it1],orientedPolygon->vertex[(it1+1)%orientedPolygon->nVertices]);
						float projectlength = tempPair.length2();
						//printf("Distance between %d of p[0] and segment %d of p[1] is %f\n",(it0+1)%p[0]->nVertices,it1,projectlength);
						if(projectlength > threshold)
						{
							if(orientedPolygon->is_inside(p[0]->vertex[(it0+1)%p[0]->nVertices]))
							{
								matchedCorners = 0;
							}
							end.x = it0;
							end.y = it1;
							break;
						}
						//accumalate error
						error = error + tempPair;
						errorCount++;
						
						//forward index
						it0 = (it0+1)%p[0]->nVertices;
						//compute rotational error
						dispStart0 = p[0]->vertex[it0] - p[0]->vertex[start0];
						dispStart1 = dispStart0 - tempPair;
						rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));
					}
					else
					{
						tempPair = orientedPolygon->vertex[(it1+1)%orientedPolygon->nVertices].projectOnSegment(p[0]->vertex[it0],p[0]->vertex[(it0+1)%p[0]->nVertices]) - orientedPolygon->vertex[(it1+1)%orientedPolygon->nVertices];
						float projectlength = tempPair.length2();
						//printf("Distance between %d of p[1] and segment %d of p[0] is %f\n",(it1+1)%orientedPolygon->nVertices,it0,projectlength);
						if(projectlength > threshold)
						{
							if(p[0]->is_inside(orientedPolygon->vertex[(it1+1)%orientedPolygon->nVertices]))
							{
								matchedCorners = 0;
							}
							end.x = it0;
							end.y = it1;
							break;
						}
						//accumalate error
						error = error + tempPair;
						errorCount++;
						matchlength += (orientedPolygon->vertex[it1] - orientedPolygon->vertex[(it1+1)%orientedPolygon->nVertices]).length2();
						//forward index
						it1 = (it1+1)%orientedPolygon->nVertices;
						//compute rotational error
						dispStart1 = orientedPolygon->vertex[it1] - orientedPolygon->vertex[start1];
						dispStart0 = tempPair + dispStart1;
						rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));
					}
				}
			}
			else
			{
				//find disp of next points
				tempPair = p[0]->vertex[(it0+1) % p[0]->nVertices] - orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices];
				//if they are close enough move forward
				if(tempPair.length2() < threshold)
				{
					//accumalate error
					error = error + tempPair;
					errorCount++;
					matchedCorners++;
					matchlength += (orientedPolygon->vertex[it1] - orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices]).length2();

					//forward indices
					it0 = (it0+1)%p[0]->nVertices;
					it1 = (it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices;
					//compute rotational error
					dispStart0 = p[0]->vertex[it0] - p[0]->vertex[start0];
					dispStart1 = orientedPolygon->vertex[it1] - orientedPolygon->vertex[start1];
					rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));

					//printf("Distance between %d of p[0] and %d of p[1] is %f\n",it0,it1, p[0]->vertex[it0].dist2(orientedPolygon->vertex[it1]));
				}
				else 
				{
					//find closer vertex
					distFromStart0 = (p[0]->vertex[(it0+1) % p[0]->nVertices] - p[0]->vertex[start0]).length2();
					distFromStart1 = (orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices] - orientedPolygon->vertex[start1]).length2();

					if(distFromStart0 < distFromStart1)
					{
						tempPair = p[0]->vertex[(it0+1)%p[0]->nVertices] - p[0]->vertex[(it0+1)%p[0]->nVertices].projectOnSegment(orientedPolygon->vertex[it1],orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices]);
						float projectlength = tempPair.length2();
						//printf("Distance between %d of p[0] and segment %d of p[1] is %f\n",(it0+1)%p[0]->nVertices,it1,projectlength);
						if(projectlength > threshold)
						{
							if(orientedPolygon->is_inside(p[0]->vertex[(it0+1)%p[0]->nVertices]))
							{
								matchedCorners = 0;
							}
							end.x = it0;
							end.y = it1;
							break;
						}
						//accumalate error
						error = error + tempPair;
						errorCount++;
						//forward index
						it0 = (it0+1)%p[0]->nVertices;
						//compute rotational error
						dispStart0 = p[0]->vertex[it0] - p[0]->vertex[start0];
						dispStart1 = dispStart0 - tempPair;
						rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));
					}
					else
					{
						tempPair = orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices].projectOnSegment(p[0]->vertex[it0],p[0]->vertex[(it0+1)%p[0]->nVertices]) - orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices];
						float projectlength = tempPair.length2();
						//printf("Distance between %d of p[1] and segment %d of p[0] is %f\n",(it1+1)%orientedPolygon->nVertices,it0,projectlength);
						if(projectlength > threshold)
						{
							if(p[0]->is_inside(orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices]))
							{
								matchedCorners = 0;
							}
							end.x = it0;
							end.y = it1;
							break;
						}
						//accumalate error
						error = error + tempPair;
						errorCount++;
						matchlength += (orientedPolygon->vertex[it1] - orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices]).length2();
						//forward index
						it1 = (it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices;
						//compute rotational error
						//compute rotational error
						dispStart1 = orientedPolygon->vertex[it1] - orientedPolygon->vertex[start1];
						dispStart0 = tempPair + dispStart1;
						rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));
					}
				}
			}
		}while(!(it0 == start.x && it1 == start.x));

		it0 = start0;
		it1 = start1;

		do
		{
			if(p[0]->order != p[1]->order)
			{
				//find disp of next points
				tempPair = p[0]->vertex[(it0-1+p[0]->nVertices) % p[0]->nVertices] - orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices];
				//if they are close enough move forward
				if(tempPair.length2() < threshold)
				{
					//accumalate error
					error = error + tempPair;
					errorCount++;
					matchedCorners++;
					matchlength += (orientedPolygon->vertex[it1] - orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices]).length2();
					//forward indices
					it0 = (it0-1+p[0]->nVertices) % p[0]->nVertices;
					it1 = (it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices;

					//compute rotational error
					dispStart0 = p[0]->vertex[it0] - p[0]->vertex[start0];
					dispStart1 = orientedPolygon->vertex[it1] - orientedPolygon->vertex[start1];
					rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));
					//printf("Distance between %d of p[0] and %d of p[1] is %f\n",it0,it1, p[0]->vertex[it0].dist2(orientedPolygon->vertex[it1]));
				}
				else 
				{
					//find closer vertex
					distFromStart0 = (p[0]->vertex[(it0-1+p[0]->nVertices) % p[0]->nVertices] - p[0]->vertex[start0]).length2();
					distFromStart1 = (orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices] - orientedPolygon->vertex[start1]).length2();

					if(distFromStart0 < distFromStart1)
					{
						tempPair = p[0]->vertex[(it0-1+p[0]->nVertices) % p[0]->nVertices] - p[0]->vertex[(it0-1+p[0]->nVertices) % p[0]->nVertices].projectOnSegment(orientedPolygon->vertex[it1],orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices]);
						float projectlength = tempPair.length2();
						//printf("Distance between %d of p[0] and segment %d of p[1] is %f\n",(it0+1)%p[0]->nVertices,it1,projectlength);
						if(projectlength > threshold)
						{
							if(orientedPolygon->is_inside(p[0]->vertex[(it0+1)%p[0]->nVertices]))
							{
								matchedCorners = 0;
							}
							start.x = it0;
							start.y = it1;
							break;
						}
						//accumalate error
						error = error + tempPair;
						errorCount++;

						//forward index
						it0 = (it0-1+p[0]->nVertices) % p[0]->nVertices;
						//compute rotational error
						dispStart0 = p[0]->vertex[it0] - p[0]->vertex[start0];
						dispStart1 = dispStart0 - tempPair;
						rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));
					}
					else
					{
						tempPair = orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices].projectOnSegment(p[0]->vertex[it0],p[0]->vertex[(it0-1+p[0]->nVertices) % p[0]->nVertices]) - orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices];
						float projectlength = tempPair.length2();
						//printf("Distance between %d of p[1] and segment %d of p[0] is %f\n",(it1+1)%orientedPolygon->nVertices,it0,projectlength);
						if(projectlength > threshold)
						{
							if(p[0]->is_inside(orientedPolygon->vertex[(it1+1)%orientedPolygon->nVertices]))
							{
								matchedCorners = 0;
							}
							start.x = it0;
							start.y = it1;
							break;
						}
						//accumalate error
						error = error + tempPair;
						errorCount++;
						matchlength += (orientedPolygon->vertex[it1] - orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices]).length2();
						//forward index
						it1 = (it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices;
						//compute rotational error
						dispStart1 = orientedPolygon->vertex[it1] - orientedPolygon->vertex[start1];
						dispStart0 = tempPair + dispStart1;
						rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));
					}
				}
			}
			else
			{
				//find disp of next points
				tempPair = p[0]->vertex[(it0-1+p[0]->nVertices) % p[0]->nVertices] - orientedPolygon->vertex[(it1+1) % orientedPolygon->nVertices];
				//if they are close enough move forward
				if(tempPair.length2() < threshold)
				{
					//accumalate error
					error = error + tempPair;
					errorCount++;
					matchedCorners++;
					matchlength += (orientedPolygon->vertex[it1] - orientedPolygon->vertex[(it1+1) % orientedPolygon->nVertices]).length2();
					//forward indices
					it0 = (it0-1+p[0]->nVertices) % p[0]->nVertices;
					it1 = (it1+1) % orientedPolygon->nVertices;
					//compute rotational error
					dispStart0 = p[0]->vertex[it0] - p[0]->vertex[start0];
					dispStart1 = orientedPolygon->vertex[it1] - orientedPolygon->vertex[start1];
					rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));

					//printf("Distance between %d of p[0] and %d of p[1] is %f\n",it0,it1, p[0]->vertex[it0].dist2(orientedPolygon->vertex[it1]));
				}
				else 
				{
					//find closer vertex
					distFromStart0 = (p[0]->vertex[(it0-1+p[0]->nVertices) % p[0]->nVertices] - p[0]->vertex[start0]).length2();
					distFromStart1 = (orientedPolygon->vertex[(it1+1) % orientedPolygon->nVertices] - orientedPolygon->vertex[start1]).length2();

					if(distFromStart0 < distFromStart1)
					{
						tempPair = p[0]->vertex[(it0-1+p[0]->nVertices) % p[0]->nVertices] - p[0]->vertex[(it0-1+p[0]->nVertices) % p[0]->nVertices].projectOnSegment(orientedPolygon->vertex[it1],orientedPolygon->vertex[(it1+1) % orientedPolygon->nVertices]);
						float projectlength = tempPair.length2();
						//printf("Distance between %d of p[0] and segment %d of p[1] is %f\n",(it0+1)%p[0]->nVertices,it1,projectlength);
						if(projectlength > threshold)
						{
							if(orientedPolygon->is_inside(p[0]->vertex[(it0+1)%p[0]->nVertices]))
							{
								matchedCorners = 0;
							}
							start.x = it0;
							start.y = it1;
							break;
						}
						//accumalate error
						error = error + tempPair;
						errorCount++;
						//forward index
						it0 = (it0-1+p[0]->nVertices) % p[0]->nVertices;
						//compute rotational error
						dispStart0 = p[0]->vertex[it0] - p[0]->vertex[start0];
						dispStart1 = dispStart0 - tempPair;
						rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));
					}
					else
					{
						tempPair = orientedPolygon->vertex[(it1+1) % orientedPolygon->nVertices].projectOnSegment(p[0]->vertex[it0],p[0]->vertex[(it0-1+p[0]->nVertices) % p[0]->nVertices]) - orientedPolygon->vertex[(it1+1) % orientedPolygon->nVertices];
						float projectlength = tempPair.length2();
						//printf("Distance between %d of p[1] and segment %d of p[0] is %f\n",(it1+1)%orientedPolygon->nVertices,it0,projectlength);
						if(projectlength > threshold)
						{
							if(p[0]->is_inside(orientedPolygon->vertex[(it1-1+orientedPolygon->nVertices) % orientedPolygon->nVertices]))
							{
								matchedCorners = 0;
							}
							start.x = it0;
							start.y = it1;
							break;
						}
						//accumalate error
						error = error + tempPair;
						errorCount++;
						matchlength += (orientedPolygon->vertex[it1] - orientedPolygon->vertex[(it1+1) % orientedPolygon->nVertices]).length2();
						//forward index
						it1 = (it1+1) % orientedPolygon->nVertices;
						//compute rotational error
						//compute rotational error
						dispStart1 = orientedPolygon->vertex[it1] - orientedPolygon->vertex[start1];
						dispStart0 = tempPair + dispStart1;
						rotError += asin(dispStart0.cross(dispStart1)/(sqrt(dispStart0.length2())*sqrt(dispStart1.length2())));
					}
				}
			}
		}while(!(it0 == end.x && it1 == end.y));

		//translate rotate
		rotError = rotError/errorCount;
		error.x /= errorCount;
		error.y /= errorCount;
		
		for(int i = 0; i < orientedPolygon->nVertices; i++)
		{
			orientedPolygon->vertex[i].x = orientedPolygon->vertex[i].x + error.x;
			orientedPolygon->vertex[i].y = orientedPolygon->vertex[i].y + error.y;	
			shift.x = orientedPolygon->vertex[i].x - orientedPolygon->vertex[start1].x;
			shift.y = orientedPolygon->vertex[i].y - orientedPolygon->vertex[start1].y;
			rot.x = shift.x * cos(rotError) + shift.y * sin(rotError);
			rot.y = -shift.x * sin(rotError) + shift.y * cos(rotError);
			orientedPolygon->vertex[i].x = rot.x + orientedPolygon->vertex[start1].x;
			orientedPolygon->vertex[i].y = rot.y + orientedPolygon->vertex[start1].y;
		}
		
		translate = !translate;
	}while(!(lastStart == start && lastEnd == end));
	MatchFeatures mFeatures;
	mFeatures.matchlength = matchlength;
	mFeatures.matchcount = matchedCorners;
	mFeatures.startindex = start;
	mFeatures.rotation = rotError;
	mFeatures.translation = shift;
	mFeatures.endindex = end;
	mFeatures.initIndex.y = start1;
	mFeatures.initIndex.x = start0;

	p[1] = orientedPolygon;
	return mFeatures;
}

Link *getAngleMatchList(Polygon **p)
{
	int r,c;
	//allocate pointers to cumulative angle matrices
	Image **angleMatrix = (Image**)calloc(2,sizeof(Image*));
	//allocate matrix for polygons
	angleMatrix[0] = new Image(p[0]->nVertices, p[0]->nVertices, CV_32F);
	angleMatrix[1] = new Image(p[1]->nVertices, p[1]->nVertices, CV_32F);

	//calculate cumalative angle matrix for polygon 1
	for(r = 0;r < p[0]->nVertices; r++)
	{
		Data(angleMatrix[0]->data, angleMatrix[0]->cols, r, 0, float) =  p[0]->angle[r];
		//printf("%f ", Data(angleMatrix[0]->data, angleMatrix[0]->cols, r, 0, float));
		for(c = 1; c < p[0]->nVertices; c++)
		{
			//compute the cumalative angles
			Data(angleMatrix[0]->data, angleMatrix[0]->cols, r, c, float) =  Data(angleMatrix[0]->data, angleMatrix[0]->cols, r, c-1, float) + p[0]->angle[(r+c)%p[0]->nVertices];
			//printf("%f ", Data(angleMatrix[0]->data, angleMatrix[0]->cols, r, c, float));
		}
		//printf("\n");
	}

	//calculate cumalative angle matrix for polygon 2 in reverse order
	if(p[0]->order == p[1]->order)
	{
		for(r = 0;r < p[1]->nVertices; r++)
		{
			Data(angleMatrix[1]->data, angleMatrix[1]->cols, r, 0, float) =  (360 - p[1]->angle[r]);
			//printf("%f ", Data(angleMatrix[1]->data, angleMatrix[1]->cols, r, c, float));
			for(c = 1; c < p[1]->nVertices; c++)
			{
				//compute the cumalative angles
				Data(angleMatrix[1]->data, angleMatrix[1]->cols, r, c, float) = Data(angleMatrix[1]->data, angleMatrix[1]->cols, r, c-1, float) + 360 - p[1]->angle[(r-c+p[1]->nVertices) % p[1]->nVertices];
				//printf("%f ", Data(angleMatrix[1]->data, angleMatrix[1]->cols, r, c, float));
			}
			//printf("\n");
		}
	}
	else
	{
		for(r = 0;r < p[1]->nVertices; r++)
		{
			Data(angleMatrix[1]->data, angleMatrix[1]->cols, r, 0, float) =  (360 - p[1]->angle[r]);
			//printf("%f ", Data(angleMatrix[1]->data, angleMatrix[1]->cols, r, 0, float));
			for(c = 1; c < p[1]->nVertices; c++)
			{
				//compute the cumalative angles
				Data(angleMatrix[1]->data, angleMatrix[1]->cols, r, c, float) = Data(angleMatrix[1]->data, angleMatrix[1]->cols, r, c-1, float) + 360 - p[1]->angle[(r+c) % p[1]->nVertices];
				//printf("%f ", Data(angleMatrix[1]->data, angleMatrix[1]->cols, r, c, float));
			}
			//printf("\n");
		}
	}
	
	//get longest angle sequence match
	int shiftR, shiftC;
	const float threshold = 30;
	float errorR, errorC, errorRC, minError;
	int maxR = 0, maxC = 0, maxShiftR = 0, maxShiftC = 0;
	Link* head = new Link(new Pixel());
	Link* current = head;
	Link* lastHead = head;
	Link* temp;
	const int matchLengthThreshold = 1;
	int matchLength = 0;
	for(r = 0; r < p[0]->nVertices; r++)
	{
		for(c = 0; c < p[1]->nVertices; c++)
		{
			shiftR = 0;
			shiftC = 0;
			minError = fabs(Data(angleMatrix[0]->data, angleMatrix[0]->cols, r, 0, float) - Data(angleMatrix[1]->data, angleMatrix[1]->cols, c, 0, float));
			matchLength = 0;
			while(true)
			{
				if(minError > threshold)
				{
					break;
				}
				if(shiftR >= p[0]->nVertices)
				{
					break;
				}
				if(shiftC >= p[1]->nVertices)
				{
					break;
				}
				//update match length
				matchLength++;
				//attach block
				if(p[1]->order != p[0]->order)
				{
					current->next = new Link(new Pixel((r+shiftR)%p[0]->nVertices, (c+shiftC)%p[1]->nVertices));
				}
				else
				{
					current->next = new Link(new Pixel((r+shiftR)%p[0]->nVertices, (c-shiftC+p[1]->nVertices)%p[1]->nVertices));
				}
				current = current->next;
				//compute errors
				errorR = fabs(Data(angleMatrix[0]->data, angleMatrix[0]->cols, r, shiftR+1, float) - Data(angleMatrix[1]->data, angleMatrix[1]->cols, c, shiftC, float) + (shiftC - shiftR - 1)*180);
				errorC = fabs(Data(angleMatrix[0]->data, angleMatrix[0]->cols, r, shiftR, float) - Data(angleMatrix[1]->data, angleMatrix[1]->cols, c, shiftC+1, float) - (shiftR - shiftC - 1)*180);
				errorRC = fabs(Data(angleMatrix[0]->data, angleMatrix[0]->cols, r, shiftR+1, float) - Data(angleMatrix[1]->data, angleMatrix[1]->cols, c, shiftC+1, float) - (shiftR - shiftC)*180);
				if(errorR < errorC)
				{
					if(errorR < errorRC)
					{
						minError = errorR;
						shiftR++;
					}
					else
					{;
						minError = errorRC;
						shiftR++;
						shiftC++;
					}
				}
				else
				{
					if(errorC < errorRC)
					{
						minError = errorC;
						shiftC++;
					}
					else
					{
						minError = errorRC;
						shiftR++;
						shiftC++;
					}
				}
			}
			if(matchLength < matchLengthThreshold)
			{
				current = lastHead->next;
				while(current != NULL)
				{
					temp = current;
					current = current->next;
					delete temp;
				}
				current = lastHead;
				lastHead->next = NULL;
			}
			else
			{
				current->next = new Link(new Pixel());
				current = current->next;
				lastHead = current;
			}

		}
	}
	return head;
}

MatchFeatures getBestMatch(Polygon **p)
{
	//local iterators
	Link *head,*current;
	head = getAngleMatchList(p);
	current = head;
	while(current != NULL)
	{
		printf("%d %d\n", current->data->x, current->data->y);
		current = current->next;
	}
	
	current = head;
	Polygon* tempPoly = p[1];
	Polygon* bestPoly = NULL;
	int matches = 0;
	int maxlength;
	int maxMatches = 0;
	drawPolygonArray(p,2,"Polygons2","polygons2.jpg");
	MatchFeatures mFeature,maxFeatures;
	while(current->next!=NULL)
	{
		p[1] = tempPoly;
		if(current->data->x == -1)
		{
			current = current->next;
			mFeature = edgeMatch(p,current->data->x,current->data->y,1600);
			matches = mFeature.matchcount;
			printf("Match length around %d %d is %d\n", current->data->x, current->data->y, matches);
			if(matches > maxMatches || (matches == maxMatches && mFeature.matchlength > maxlength))
			{
					if(bestPoly)
						delete bestPoly;
					bestPoly = p[1];
					maxMatches = matches;
					maxlength = mFeature.matchlength;
					maxFeatures = mFeature;
			}
			else
			{
				delete p[1];
			}
		}
		else
		{
			current = current->next;
		}
	}
	
	//matches = edgeMatch(p,12,5,2500);
	p[1] = bestPoly;
	//maxMatches = edgeMatch(p,1,9,1600);
	//tempPoly = p[1];
	//delete tempPoly;
	//drawPolygonArray(p,2,"Polygons","polygons.jpg");
	
	p[1] = tempPoly->reorientTo(p[0],9,1);
	//drawPolygonArray(p,2,"Polygons2","polygons2.jpg");
	//waitKey(0);
	return maxFeatures;
}

int main()
{
	//iterator
	int i;
	//filenames
	char filename[256];;
	//allocate polygon pointers
	Polygon **p = (Polygon**)calloc(2,sizeof(Polygon*));
	//load polygons from files
	for(i = 0; i < 2; i++)
	{
		do
		{
			printf("Enter Filename %d:", i);
			scanf("%s",filename);
			p[i] = new Polygon(filename);

		}while(p[i]->image == NULL);
	}

	//Display masked images
	for(i = 0; i < 2; i++)
	{
		cv::Mat tempImage = *(p[i]->maskedImage->toMat());
		sprintf(filename, "Masked image %d", i);
		cv::imshow(filename, tempImage);
		sprintf(filename, "%s.jpg", filename);
		cv::imwrite(filename, tempImage);
	}
	Polygon *tempPoly = p[1];
	MatchFeatures mFeature = getBestMatch(p);
	
	Polygon *tPoly = p[1];
	p[1] = tempPoly;
	
	p[1] = p[1]->reorientTo(p[0],mFeature.initIndex.y,mFeature.initIndex.x);
	
	delete tPoly;
	Pair error = mFeature.translation;
	float rotError = mFeature.rotation;
	Pair rot,shift;
	int start1 = mFeature.initIndex.y;
	for(int i = 0; i < p[1]->nVertices; i++)
	{
		p[1]->vertex[i].x = p[1]->vertex[i].x + error.x;
		p[1]->vertex[i].y = p[1]->vertex[i].y + error.y;	
		shift.x = p[1]->vertex[i].x - p[1]->vertex[start1].x;
		shift.y = p[1]->vertex[i].y - p[1]->vertex[start1].y;
		rot.x = shift.x * cos(rotError) + shift.y * sin(rotError);
		rot.y = -shift.x * sin(rotError) + shift.y * cos(rotError);
		p[1]->vertex[i].x = rot.x + p[1]->vertex[start1].x;
		p[1]->vertex[i].y = rot.y + p[1]->vertex[start1].y;
	}
	printf("Match length around %d %d is %d\n", mFeature.startindex.x, mFeature.startindex.y, mFeature.matchcount);
	drawPolygonArray(p,2,"Polygons2","polygons2.jpg");
	return 0;
}
