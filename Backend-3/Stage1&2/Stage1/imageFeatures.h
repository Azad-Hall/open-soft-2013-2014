#ifndef __IMAGEFEATURES_H_INCLUDED__
#define __IMAGEFEATURES_H_INCLUDED__

//forward declared dependencies

//included headers
#include "Image.h"
#include "Utility.h"

using namespace cv;

class ImageFeatures
{
private:
	
public:
	ImageFeatures();
	inline static unsigned int* Histogram(Image* image)
	{
		//allocate the histogram
		unsigned int* histogram = (unsigned int*)calloc(256, sizeof(unsigned int));

		//generate the histogram
		int r,c;
		for(r = 0; r < image->rows; r++)
		{
			for(c = 0; c < image->cols; c++)
			{
				histogram[Data(image->data, image->cols, r, c, uchar)]++;
			}
		}

		//return the histogram
		return histogram;
	}

	static Image* Entropy(Image* image, unsigned char basis);
};

#endif
