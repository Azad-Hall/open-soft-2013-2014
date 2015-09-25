#ifndef __IMAGEPROCESSING_H_INCLUDED__
#define __IMAGEPROCESSING_H_INCLUDED__

//forward declared dependencies

//included headers
#include "Image.h"
#include "Utility.h"

using namespace cv;

class ImageProcessing
{
private:
	Image* image;
	Image* grayImage;
	Image* thresholdedImage;

	Image* distTransform(Image* input, int edge);
	//Image* getDilatedImage(Image* thresholdedImage,int kernelSize = 3);
	Image* getDilatedImage(Image* input, int kernelSize = 3);
	Image* getErodedImage(Image* input, int kernelSize = 3);
	Image* getOpenedImage(Image* input, int kernelSize = 3);
	Image* getClosedImage(Image* input, int kernelSize = 3);
public:
	ImageProcessing();
	ImageProcessing(Image* image_);

	Image* getGrayscaleImage();
	int calculateThreshold();
	Image* getThresholdedImage(bool direction=true);
	Image* getThresholdedImage(float threshold, bool direction=true);
	Image* doMorphologicalOperation(int mode,int kernelSize = 3);
	Image* doMorphologicalOperation(int mode,int kernelSize,int threshold,bool direction);
	Image* label(Image* thresholdedImage);
	Link* polyApprox(Image* image, float threshold, float edgeLengthThreshold = 0);
	Image* RefineForeground(Image* image, Image** mask);
	~ImageProcessing()
	{
		if(thresholdedImage)
		{
			delete thresholdedImage;
		}
	}
};

#endif