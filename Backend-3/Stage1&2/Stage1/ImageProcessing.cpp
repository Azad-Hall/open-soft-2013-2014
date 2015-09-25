#include "imageProcessing.h"
#include "morphologyConstants.h"
#include "imageFeatures.h"

#include <opencv2/core/core.hpp>
#include <stdio.h>

ImageProcessing::ImageProcessing()
{}

ImageProcessing::ImageProcessing(Image* image_):
image(image_),
grayImage(new Image(0,0, CV_8U,1)),
thresholdedImage(new Image(0,0, CV_8U,1))
{}

//Develop this fully

Image* ImageProcessing::getGrayscaleImage()
{
	if(grayImage->rows==0 && grayImage->cols==0)
	{
		if(image->channels==1)
			grayImage = image;
		else
		{
			//TODO: color to gray conversion
			Mat imageCV = *(image->toMat());
			Mat grayImageCV;
			//cv::col(imageCV, grayImageCV, CV_BGR2GRAY);
			grayImage = new Image(grayImageCV);
		}
	}
	return grayImage;
}

int ImageProcessing::calculateThreshold()
{
	getGrayscaleImage();

	//get the histogram

	ImageFeatures im = ImageFeatures();
	unsigned int * histogram = im.Histogram(grayImage);

	//calculate the threshold
	float maxVariance = 0, variance, meanLeft = 0, meanRight = 0;
	int i, threshold = 0, pixelsLeft = 0, pixelsRight = 0;
	int sumPixels;
	//weighted sum
	for(i = 0; i < 256; i++)
	{
		meanRight += i * histogram[i];
		pixelsRight += histogram[i];
	}
	sumPixels = pixelsRight;
	//normalize
	meanRight = meanRight/pixelsRight;
	//maximimse variance
	for(i = 0; i < 255; i++)
	{
		//update means
		meanLeft = (meanLeft * pixelsLeft) + i * histogram[i];
		meanRight = (meanRight * pixelsRight) - i * histogram[i];
		//update sizes
		pixelsLeft += histogram[i];
		pixelsRight -= histogram[i];
		if(pixelsLeft > 0)
		{
			//normalize means
			meanLeft = meanLeft/((float)(pixelsLeft)/sumPixels);
		}
		meanRight = meanRight/((float)pixelsRight/sumPixels);
		//calculate variance
		//variance = pixelsLeft * pixelsRight * (meanLeft - meanRight) * (meanLeft - meanRight);
		variance = pixelsLeft * (meanLeft*meanLeft) + pixelsRight*(meanRight*meanRight);
		//update maximum variance and threshold
		if(variance > maxVariance)
		{
			maxVariance = variance;
			threshold = i;
		}
	}
	return threshold;
}

Image* ImageProcessing::getThresholdedImage(bool direction)
{
	if(thresholdedImage->rows!=0 || thresholdedImage->cols!=0)
		return thresholdedImage;

	int threshold = calculateThreshold();
	return getThresholdedImage(threshold,direction);
}

Image* ImageProcessing::getThresholdedImage(float threshold, bool direction)
{
	if(thresholdedImage->rows!=0 || thresholdedImage->cols!=0)
		return thresholdedImage;

	getGrayscaleImage();
	//allocate thresholded image
	thresholdedImage = new Image(grayImage->rows, grayImage->cols, CV_8U);

	//threshold the image
	int r,c;
	for(r = 0; r < grayImage->rows; r++)
	{
		for(c = 0; c < grayImage->cols; c++)
		{
			if(Data(grayImage->data, grayImage->cols, r, c, uchar) > threshold)
			{
				Data(thresholdedImage->data, thresholdedImage->cols, r, c, uchar) = direction? 255 : 0;
			}
			else
			{
				Data(thresholdedImage->data, thresholdedImage->cols, r, c, uchar) = direction? 0 : 255;
			}
		}
	}
	//return the thresholded image
	return thresholdedImage;
}

//default for optimal threshold

Image* ImageProcessing::doMorphologicalOperation(int mode,int kernelSize)
{
	thresholdedImage = getThresholdedImage();

	switch(mode)
	{
		case MORPHO_OPEN:
			return getOpenedImage(thresholdedImage,kernelSize);
		break;

		case MORPHO_CLOSE:
			return getClosedImage(thresholdedImage,kernelSize);
		break;

		case MORPHO_DILATE:
			return getDilatedImage(thresholdedImage,kernelSize);
		break;

		case MORPHO_ERODE:
			return getErodedImage(thresholdedImage,kernelSize);
		break;
	}
}

Image* ImageProcessing::doMorphologicalOperation(int mode,int kernelSize,int threshold,bool direction)
{
	thresholdedImage = getThresholdedImage(threshold,direction);

	switch(mode)
	{
		case MORPHO_OPEN:
			return getOpenedImage(thresholdedImage,kernelSize);
		break;

		case MORPHO_CLOSE:
			return getClosedImage(thresholdedImage,kernelSize);
		break;

		case MORPHO_DILATE:
			return getDilatedImage(thresholdedImage,kernelSize);
		break;

		case MORPHO_ERODE:
			return getErodedImage(thresholdedImage,kernelSize);
		break;
	}
}


Image* ImageProcessing::distTransform(Image* input, int edge)
{
	int r,c;
	int v1,v2,v3;
	Image* output = new Image(input->rows, input->cols, CV_16U);
	for(r = 0; r < input->rows; r++)
	{
		for(c = 0; c < input->cols; c++)
		{
			if(Data(input->data, input->cols, r, c, uchar) == edge)
			{
				Data(output->data, output->cols, r, c, ushort) = 0;
			}
			else
			{
				if(r > 0)
				{
					v1 = Data(output->data, output->cols, r-1, c, ushort);
				}
				else
				{
					v1 = input->rows + input->cols - 2;
				}
				if(c > 0)
				{
					v2 = Data(output->data, output->cols, r, c-1, ushort);
				}
				else
				{
					v2 = input->rows + input->cols - 2;
				}

				if(v1 < v2)
				{
					Data(output->data, output->cols, r, c, ushort) = v1 + 1;
				}
				else
				{
					Data(output->data, output->cols, r, c, ushort) = v2 + 1;
				}
			}
		}
	}
	for(r = input->rows-2; r >= 0; r--)
	{
		for(c = input->cols-2; c >= 0; c--)
		{
			if(Data(input->data, input->cols, r, c, uchar) != edge)
			{
				v1 = Data(output->data, output->cols, r+1, c, ushort);
				v2 = Data(output->data, output->cols, r, c+1, ushort);
				if(v1 < v2)
				{
					v3 = v1 + 1;
				}
				else
				{
					v3 = v2 + 1;
				}
				if(Data(output->data, output->cols, r, c, ushort) > v3)
				{
					Data(output->data, output->cols, r, c, ushort) = v3;
				}
			}
		}
	}
	return output;
}

Image* ImageProcessing::getDilatedImage(Image* input, int kernelSize)
{
	int r,c;
	Image* dist = distTransform(input, 255);
	Image* output = new Image(input->rows, input->cols);

	for(r = 0; r < input->rows; r++)
	{
		for(c = 0; c < input->cols; c++)
		{
			if(Data(dist->data, dist->cols, r, c, ushort) <= kernelSize)
			{
				Data(output->data, output->cols, r, c, uchar) = 255;
			}
			else
			{
				Data(output->data, output->cols, r, c, uchar) = 0;
			}
		}
	}

	delete dist;
	return output;
}

Image* ImageProcessing::getErodedImage(Image* input, int kernelSize)
{
	int r,c;
	Image* dist = distTransform(input, 0);
	Image* output = new Image(input->rows, input->cols);

	for(r = 0; r < input->rows; r++)
	{
		for(c = 0; c < input->cols; c++)
		{
			if(Data(dist->data, dist->cols, r, c, ushort) <= kernelSize)
			{
				Data(output->data, output->cols, r, c, uchar) = 0;
			}
			else
			{
				Data(output->data, output->cols, r, c, uchar) = 255;
			}
		}
	}

	delete dist;
	return output;
}

Image* ImageProcessing::getOpenedImage(Image* input, int kernelSize)
{
	Image* erodedInput = getErodedImage(input, kernelSize);
	Image* output = getDilatedImage(erodedInput, kernelSize);
	delete erodedInput;
	return output;
}

Image* ImageProcessing::getClosedImage(Image* input, int kernelSize)
{
	Image* dilatedInput = getDilatedImage(input, kernelSize);
	Image* output = getErodedImage(dilatedInput, kernelSize);
	delete dilatedInput;
	return output;
}

Image* ImageProcessing::label(Image* thresholdedImage)
{
	Image* labelledImage = new Image(thresholdedImage->rows, thresholdedImage->cols, CV_16U);
	
	//initialize the labelled image
	int r,c;

	//label the image
	int regionArea = 0, remainingArea = thresholdedImage->rows * thresholdedImage->cols;
	int maxRegionArea = 0, largestRegion = 0;
	Stack* pixelStack;
	Pixel* p = NULL;
	int label = 0;
	pixelStack = new Stack(remainingArea);
	Pixel *tempP;
	for(r = 0; r < thresholdedImage->rows; r++)
	{
		for(c = 0; c < thresholdedImage->cols; c++)
		{
			remainingArea--;
			//skip if already labelled
			if(Data(thresholdedImage->data, thresholdedImage->cols, r, c, uchar) > 0 && Data(labelledImage->data, labelledImage->cols, r, c, ushort) == 0)
			{
				regionArea = 0;
				label++;
				//iterate till all connected componetnts are labelled
				p = new Pixel();
				p->x = r;
				p->y = c;
				Data(labelledImage->data, labelledImage->cols, p->x, p->y, ushort) = label;
				while(p)
				{
					regionArea++;
					//check neighbours and add to stack
					if(p->x > 0 && Data(labelledImage->data, labelledImage->cols, p->x-1, p->y, ushort) == 0 && Data(thresholdedImage->data, thresholdedImage->cols, p->x-1, p->y, uchar) > 0)
					{
						pixelStack->Push(Pixel(p->x - 1, p->y));
						Data(labelledImage->data, labelledImage->cols, p->x-1, p->y, ushort) = label;
					}
					if(p->y > 0 && Data(labelledImage->data, labelledImage->cols, p->x, p->y-1, ushort) == 0 && Data(thresholdedImage->data, thresholdedImage->cols, p->x, p->y-1, uchar) > 0)
					{
						pixelStack->Push(Pixel(p->x, p->y - 1));
						Data(labelledImage->data, labelledImage->cols, p->x, p->y-1, ushort) = label;
					}
					if(p->x < (thresholdedImage->rows - 1) && Data(labelledImage->data, labelledImage->cols, p->x+1, p->y, ushort) == 0 && Data(thresholdedImage->data, thresholdedImage->cols, p->x+1, p->y, uchar) > 0)
					{
						pixelStack->Push(Pixel(p->x + 1, p->y));
						Data(labelledImage->data, labelledImage->cols, p->x+1, p->y, ushort) = label;
					}
					if(p->y < (thresholdedImage->cols - 1) && Data(labelledImage->data, labelledImage->cols, p->x, p->y+1, ushort) == 0 && Data(thresholdedImage->data, thresholdedImage->cols, p->x, p->y+1, uchar) > 0)
					{
						pixelStack->Push(Pixel(p->x, p->y + 1));
						Data(labelledImage->data, labelledImage->cols, p->x, p->y+1, ushort) = label;
					}
					//get new pixel from stack
					tempP = pixelStack->Pop();
					if(tempP)
					{
						p->x = tempP->x;
						p->y = tempP->y;
					}
					else
					{
						delete p;
						break;
					}
				}
			}
			
			if(regionArea > maxRegionArea)
			{
				maxRegionArea = regionArea;
				largestRegion = label;
			}
			if(maxRegionArea > remainingArea)
			{
				break;
			}
		}
		if(maxRegionArea > remainingArea)
		{
			break;
		}
	}

	Image* region = new Image(thresholdedImage->rows, thresholdedImage->cols, CV_8U);
	//Second pass to extract largest region
	for(r = 0; r < thresholdedImage->rows; r++)
	{
		for(c = 0; c < thresholdedImage->cols; c++)
		{
			if(Data(labelledImage->data, labelledImage->cols, r, c, ushort) == largestRegion)
			{
				Data(region->data, region->cols, r, c, uchar) = 255;
			}
			else
			{
				Data(region->data, region->cols, r, c, uchar) = 0;
			}
		}
	}

	//delete the stack
	delete pixelStack;
	delete labelledImage;
	return region;
}

//TODO:MaxEDGELENGTH
Link* ImageProcessing::polyApprox(Image* image, float threshold, float edgeLengthThreshold)
{
	Link* corner;
	//distance transform
	Image* dist = distTransform(image, 0);
	//interation variables
	int r, c, count = 0;
	//Start point
	Pixel* start = new Pixel();
	Pixel* current = new Pixel();
	//find starting pixel
	for(r = 0; r < image->rows; r++)
	{
		for(c = 0; c < image->cols; c++)
		{
			if(Data(dist->data, dist->cols, r, c, ushort) == 1)
			{
				start->x = c;
				start->y = r;
				break;
			}
		}
		if(start->x > -1)
		{
			break;
		}
	}
	//find mean , second moment
	float meanR = 0, meanC = 0, sigmaRR = 0, sigmaRC = 0, sigmaCC = 0;
	current->x = start->x;
	current->y = start->y;
	while(true)
	{
		meanR += current->y;
		meanC += current->x;
		sigmaRR += current->y*current->y;
		sigmaRC += current->y*current->x;
		sigmaCC += current->x*current->x;
		count++;
		Data(dist->data, dist->cols, current->y, current->x, short) = -1;
		//find next boundry point
		if(current->y > 0 && Data(dist->data, dist->cols, current->y-1, current->x, ushort) == 1)
		{
			current->y = current->y-1;
		}
		else if(current->x > 0 && Data(dist->data, dist->cols, current->y, current->x-1, ushort) == 1)
		{
			current->x = current->x-1;
		}
		else if(current->y < (dist->rows-1) && Data(dist->data, dist->cols, current->y+1, current->x, ushort) == 1)
		{
			current->y = current->y+1;
		}
		else if(current->x < (dist->cols-1) && Data(dist->data, dist->cols, current->y, current->x+1, ushort) == 1)
		{
			current->x = current->x+1;
		}
		else if(current->y > 0 && current->x > 0 && Data(dist->data, dist->cols, current->y-1, current->x-1, ushort) == 1)
		{
			current->y = current->y-1;
			current->x = current->x-1;
		}
		else if(current->y > 0 && current->x < (image->cols-1) && Data(dist->data, dist->cols, current->y-1, current->x+1, ushort) == 1)
		{
			current->y = current->y-1;
			current->x = current->x+1;
		}
		else if(current->y < (image->rows-1) && current->x > 0 && Data(dist->data, dist->cols, current->y+1, current->x-1, ushort) == 1)
		{
			current->y = current->y+1;
			current->x = current->x-1;
		}
		else if(current->y < (image->rows-1) && current->x < (image->cols-1) && Data(dist->data, dist->cols, current->y+1, current->x+1, ushort) == 1)
		{
			current->y = current->y+1;
			current->x = current->x+1;
		}
		else
		{
			break;
		}
	}
	//mean normalize
	meanR /= count;
	meanC /= count;
	//second moment normalize
	sigmaRR /= count;
	sigmaRC /= count;
	sigmaCC /= count;

	//covariance matrix
	float covRR, covRC, covCC;
	covRR = sigmaRR - meanR*meanR;
	covRC = sigmaRC - meanR*meanC;
	covCC = sigmaCC - meanC*meanC;

	//p*l^2 + q*l + r = 0
	float c1 = 1;
	float c2 = -(covRR + covCC);
	float c3 = covRR*covCC - covRC*covRC;

	//eigenvalues
	float lambda = (-c2 + sqrt(c2*c2-4*c1*c3))/(2*c1);

	//eigen vector [1 x]
	float x = (lambda - covRR)/covRC;

	//iteration variable
	float projection;
	float maxProjection = meanR + meanC*x;
	float minProjection = maxProjection;
	//maximally distant points
	current->x = start->x;
	current->y = start->y;
	//Setup corner point linked list
	corner = new Link(new Pixel());
	corner->next = new Link(new Pixel());
	//linearize the boundry
	Pixel* linearBoundry = (Pixel*)calloc(count, sizeof(Pixel));
	count = 0;
	while(true)
	{
		linearBoundry[count].x = current->x;
		linearBoundry[count].y = current->y;
		count++;
		projection = current->y + current->x*x;
		if(projection > maxProjection)
		{
			maxProjection = projection;
			corner->data->x = current->x;
			corner->data->y = current->y;
		}
		if(projection < minProjection)
		{
			minProjection = projection;
			corner->next->data->x = current->x;
			corner->next->data->y = current->y;
		}

		Data(dist->data, dist->cols, current->y, current->x, ushort) = 1;
		//find next boundry point
		if(current->y > 0 && Data(dist->data, dist->cols, current->y-1, current->x, short) == -1)
		{
			current->y = current->y-1;
		}
		else if(current->x > 0 && Data(dist->data, dist->cols, current->y, current->x-1, short) == -1)
		{
			current->x = current->x-1;
		}
		else if(current->y < (dist->rows-1) && Data(dist->data, dist->cols, current->y+1, current->x, short) == -1)
		{
			current->y = current->y+1;
		}
		else if(current->x < (dist->cols-1) && Data(dist->data, dist->cols, current->y, current->x+1, short) == -1)
		{
			current->x = current->x+1;
		}
		else if(current->y > 0 && current->x > 0 && Data(dist->data, dist->cols, current->y-1, current->x-1, short) == -1)
		{
			current->y = current->y-1;
			current->x = current->x-1;
		}
		else if(current->y > 0 && current->x < (image->cols-1) && Data(dist->data, dist->cols, current->y-1, current->x+1, short) == -1)
		{
			current->y = current->y-1;
			current->x = current->x+1;
		}
		else if(current->y < (image->rows-1) && current->x > 0 && Data(dist->data, dist->cols, current->y+1, current->x-1, short) == -1)
		{
			current->y = current->y+1;
			current->x = current->x-1;
		}
		else if(current->y < (image->rows-1) && current->x < (image->cols-1) && Data(dist->data, dist->cols, current->y+1, current->x+1, short) == -1)
		{
			current->y = current->y+1;
			current->x = current->x+1;
		}
		else
		{
			break;
		}
	}
	
	Link* currentCorner = corner;
	Link* nextCorner;
	float directionX, directionY;
	int maxErrorPoint;
	float dirVecLen;
	int currentPos, lastCorner;
	float edgeLength;
	//navigate the first corner
	for(currentPos = 0; currentPos < count; currentPos++)
	{
		if(currentCorner->data->x == linearBoundry[currentPos].x && currentCorner->data->y == linearBoundry[currentPos].y)
		{
			break;
		}
	}


	while(currentCorner != NULL)
	{
		//calculate direction vector perpendicular to edge
		nextCorner = currentCorner->next;
		if(nextCorner == NULL)
		{
			nextCorner = corner;
		}
		directionX = nextCorner->data->y - currentCorner->data->y;
		directionY = currentCorner->data->x - nextCorner->data->x;
		dirVecLen = sqrt(float(directionX*directionX + directionY*directionY));
		directionX /= dirVecLen;
		directionY /= dirVecLen;


		//set current to current corner
		maxProjection = 0;
		lastCorner = currentPos;
		for(; !(nextCorner->data->x == linearBoundry[currentPos].x && nextCorner->data->y == linearBoundry[currentPos].y); currentPos = ((++currentPos)%count))
		{
			projection = fabs((linearBoundry[currentPos].x - currentCorner->data->x)*directionX + (linearBoundry[currentPos].y - currentCorner->data->y)*directionY);
			if(projection > maxProjection)
			{
				maxProjection = projection;
				maxErrorPoint = currentPos;
			}
		}
		
		if(edgeLengthThreshold > 0)
		{
			edgeLength = (currentCorner->data->x - nextCorner->data->x)*(currentCorner->data->x - nextCorner->data->x) + (currentCorner->data->y - nextCorner->data->y)*(currentCorner->data->y - nextCorner->data->y);
		}
		else
		{
			edgeLength = 0;
		}
		if(maxProjection > threshold || edgeLength > edgeLengthThreshold)
		{
			currentPos = lastCorner;
			currentCorner->next = new Link(new Pixel(linearBoundry[maxErrorPoint].x, linearBoundry[maxErrorPoint].y),currentCorner->next);
		}
		else
		{
			currentCorner = currentCorner->next;
		}
	}

	currentCorner = corner;
	while(currentCorner != NULL)
	{
		//printf("%d %d\n", currentCorner->data->y, currentCorner->data->x);
		currentCorner = currentCorner->next;
	}

	free(linearBoundry);
	delete start;
	delete current;
	delete dist;
	return corner;
}

Image* ImageProcessing::RefineForeground(Image* image, Image** mask)
{
	Image* foregroundImage = new Image(image->rows, image->cols, CV_8U);
	Image* dist = distTransform((*mask),0);
	int r,c;
	for(r = 0; r < image->rows; r++)
	{
		for(c = 0; c < image->cols; c++)
		{
			Data(foregroundImage->data, foregroundImage->cols, r, c, uchar) = Data(image->data, image->cols, r, c, uchar) & Data((*mask)->data, (*mask)->cols, r, c, uchar);
		}
	}

	
	Stack* pixelStack = new Stack(foregroundImage->cols * foregroundImage->rows);
	Pixel* p;
	Pixel* tempPixel;
	for(r = 0; r < foregroundImage->rows; r++)
	{
		for(c = 0; c < foregroundImage->cols; c++)
		{
			
			//skip if already labelled
			if(Data(foregroundImage->data, foregroundImage->cols, r, c, uchar) >= 253 && Data(dist->data, dist->cols, r, c, ushort) == 1)
			{
				
				//iterate till all connected componetnts are labelled
				p = new Pixel();
				p->x = r;
				p->y = c;
				Data(foregroundImage->data, foregroundImage->cols, r, c, uchar) = 0;
				while(p)
				{
					//check neighbours and add to stack
					if(p->x > 0 && Data(foregroundImage->data, foregroundImage->cols, p->x-1, p->y, uchar) >= 253)
					{
						pixelStack->Push(Pixel(p->x - 1, p->y));
						Data(foregroundImage->data, foregroundImage->cols, p->x-1, p->y, uchar) = 0;
						Data((*mask)->data, (*mask)->cols, p->x-1, p->y, uchar) = 0;
					}
					if(p->y > 0 && Data(foregroundImage->data, foregroundImage->cols, p->x, p->y-1, uchar) >= 253)
					{
						pixelStack->Push(Pixel(p->x, p->y - 1));
						Data(foregroundImage->data, foregroundImage->cols, p->x, p->y-1, uchar) = 0;
						Data((*mask)->data, (*mask)->cols, p->x, p->y-1, uchar) = 0;
					}
					if(p->x < (foregroundImage->rows - 1) && Data(foregroundImage->data, foregroundImage->cols, p->x+1, p->y, uchar) >= 253)
					{
						pixelStack->Push(Pixel(p->x + 1, p->y));
						Data(foregroundImage->data, foregroundImage->cols, p->x+1, p->y, uchar) = 0;
						Data((*mask)->data, (*mask)->cols, p->x+1, p->y, uchar) = 0;
					}
					if(p->y < (foregroundImage->cols - 1) && Data(foregroundImage->data, foregroundImage->cols, p->x, p->y+1, uchar) >= 253)
					{
						pixelStack->Push(Pixel(p->x, p->y + 1));
						Data(foregroundImage->data, foregroundImage->cols, p->x, p->y+1, uchar) = 0;
						Data((*mask)->data, (*mask)->cols, p->x, p->y+1, uchar) = 0;
					}
					//get new pixel from stack
					tempPixel = pixelStack->Pop();
					if(tempPixel)
					{
						p->x = tempPixel->x;
						p->y = tempPixel->y;
					}
					else
					{
						delete p;
						break;
					}
				}
			}
		}
	}
	
	Image* oldmask = (*mask);
	(*mask) = getOpenedImage(oldmask, 20);
	
	for(r = 0; r < foregroundImage->rows; r++)
	{
		for(c = 0; c < foregroundImage->cols; c++)
		{
			Data(foregroundImage->data, foregroundImage->cols, r, c, uchar) = Data(image->data, image->cols, r, c, uchar) & Data((*mask)->data, (*mask)->cols, r, c, uchar);
		}
	}

	delete oldmask;
	delete pixelStack;
	delete dist;
	return foregroundImage;
}
