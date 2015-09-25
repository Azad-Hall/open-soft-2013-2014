#include "imageFeatures.h"
#include <math.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>

ImageFeatures::ImageFeatures()
{}

Image* ImageFeatures::Entropy(Image* image, unsigned char basis)
{
	//Get image dimensions
	int h = image->rows;
	int w = image->cols;
	//initialize iterators
	int r = 0;
	int c = 0;
	float temp;
	//precompute log
	int b2 = basis*basis;
	float *logArray = (float*)malloc(sizeof(float)*(b2+1));
	for(c = 1; c <= b2; c++)
	{
		logArray[c] = log(float(c));
	}
	int left, right, top, bottom, area, row, col;
	//allocate the histogram
	unsigned int* histogram = (unsigned int*)calloc(256, sizeof(unsigned int));
	//allocate output image
	Image* output = new Image(h, w, CV_8U);
	//adjust basis
	basis = basis>>1;
	area = 0;
	c = 0;
	//constant
	float log_256 = log(256.0);

	for(r = 0; r < h; r++)
	{
		if(r > 0)
		{
			left = 0;
			right = (basis >= w)?(w-1):basis;
			row = r+basis;
			if(row < h)
			{
				for(col = left; col <= right; col++)
				{
					area++;
					histogram[Data(image->data, image->cols, row, col, uchar)]++;
				}
			}
			row = r-basis-1;
			if(row >= 0)
			{
				for(col = left; col <= right; col++)
				{
					area--;
					histogram[Data(image->data, image->cols, row, col, uchar)]--;
				}
			}
		}
		else
		{
			left = 0;
			right = (basis >= w)?(w-1):basis;
			top = 0;
			bottom = (basis >= h)?(h-1):basis;
			for(row = top; row <= bottom; row++)
			{
				for(col = left; col <= right; col++)
				{
					area++;
					histogram[Data(image->data, image->cols, row, col, uchar)]++;
				}
			}
		}

		for(c = 0; c < w; c++)
		{
			top  = (r - basis < 0)?0:(r - basis);
			bottom = (r + basis >= h)?(h-1):(r + basis);
			
			if(c > 0)
			{
				//calculate histogram
				col = c+basis;
				if(col < w)
				{
					for(row = top; row <= bottom; row++)
					{
						area++;
						histogram[Data(image->data, image->cols, row, col, uchar)]++;
					}
				}
				col = c-basis-1;
				if(col >= 0)
				{
					for(row = top; row <= bottom; row++)
					{
						area--;
						histogram[Data(image->data, image->cols, row, col, uchar)]--;
					}
				}
			}

			//initialize output pixel
			temp = 0;
			//Data(output->data, output->cols, r, c, float) = 0;
			//calculate output pixel
			for(col = 0; col < 256; col++)
			{
				if(histogram[col])
				{
					temp += logArray[histogram[col]] * histogram[col];
					//Data(output->data, output->cols, r, c, float) += logArray[histogram[col]] * histogram[col];
				}
			}
			//Data(output->data, output->cols, r, c, float) = log(float(area))/log_256 - Data(output->data, output->cols, r, c, float)/(area*log_256);
			temp = log(float(area))/log_256 - temp/(area*log_256);
			Data(output->data, output->cols, r, c, uchar) = (unsigned char)(255*temp);
		}

		r++;
		if(r == h)
			break;
		left = (w - basis - 1 < 0)?0:(w - basis - 1);
		right = w - 1;
		//right = (c + basis >= w)?(w-1):(c + basis);	
		row = r+basis;
		if(row < h)
		{
			for(col = left; col <= right; col++)
			{
				area++;
				histogram[Data(image->data, image->cols, row, col, uchar)]++;
			}
		}
		row = r-basis-1;
		if(row >= 0)
		{
			for(col = left; col <= right; col++)
			{
				area--;
				histogram[Data(image->data, image->cols, row, col, uchar)]--;
			}
		}

		for(c = w-1; c >= 0; c--)
		{
			top  = (r - basis < 0)?0:(r - basis);
			bottom = (r + basis >= h)?(h-1):(r + basis);
			
			if(c < w - 1)
			{
				//calculate histogram
				col = c-basis;
				if(col >= 0)
				{
					for(row = top; row <= bottom; row++)
					{
						area++;
						histogram[Data(image->data, image->cols, row, col, uchar)]++;
					}
				}
				col = c+basis+1;
				if(col < w)
				{
					for(row = top; row <= bottom; row++)
					{
						area--;
						histogram[Data(image->data, image->cols, row, col, uchar)]--;
					}
				}
			}

			//initialize output pixel
			temp = 0;
			//Data(output->data, output->cols, r, c, float) = 0;
			//TODO: parallelize
			//calculate output pixel
			for(col = 0; col < 256; col++)
			{
				if(histogram[col])
				{
					temp += logArray[histogram[col]] * histogram[col];
					//Data(output->data, output->cols, r, c, float) += logArray[histogram[col]] * histogram[col];
				}
			}
			//Data(output->data, output->cols, r, c, float) = log(float(area))/log(256.0) - Data(output->data, output->cols, r, c, float)/(area*log(256.0));
			temp = log(float(area))/log(256.0) - temp/(area*log(256.0));
			Data(output->data, output->cols, r, c, uchar) = (unsigned char)(255*temp);
		}

		//printf("%d of %d rows complete\n", r, h);
	}
	return output;
}
