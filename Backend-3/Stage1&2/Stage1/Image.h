#include <stdio.h>
#include <opencv2/core/core.hpp>

#ifndef __IMAGE_H
#define __IMAGE_H

#define Data(d,w,r,c,t) ((t*)d)[(r)*w + (c)]

	class Image
	{
	public:
		int rows;
		int cols;
		int channels;
		void* data;
		unsigned char type;

		Image()
		{
			rows = 0;
			cols = 0;
			channels = 1;
			data = NULL;
			type = 0;
		}

		Image(int rows,int cols,unsigned char type = CV_8U, unsigned char channels = 1)
		{
			//class variables
			this->rows = rows;
			this->cols = cols;
			this->channels = channels;
			this->type = type;
			//allocate iamge data
			switch(type)
			{
			case CV_8U:
				data = calloc(rows*cols, sizeof(uchar));
				break;
			case CV_8S:
				data = calloc(rows*cols, sizeof(char));
				break;
			case CV_16U:
				data = calloc(rows*cols, sizeof(ushort));
				break;
			case CV_16S:
				data = calloc(rows*cols, sizeof(short));
				break;
			case CV_32S:
				data = calloc(rows*cols, sizeof(int));
				break;
			case CV_32F:
				data = calloc(rows*cols, sizeof(float));
				break;
			case CV_64F:
				data = calloc(rows*cols, sizeof(double));
				break;
			}
		}

		Image(cv::Mat image,unsigned char type = CV_8U, unsigned char channels = 1)
		{
			//class variables
			rows = image.rows;
			cols = image.cols;
			this->channels = channels;
			this->type = type;
			//allocate iamge data
			data = image.data;
		}

		cv::Mat* toMat()
		{
			//allocate matrix
			cv::Mat* output = new cv::Mat(rows, cols, type);
			//copy from data to mat
			output->data = (uchar*)data;
			return output;
		}

		Image* zeropad()
		{
			Image* image = new Image(rows+2, cols+2, type, channels);
			
			//allocate iamge data
			switch(type)
			{
			case CV_8U:
				image->data = calloc(image->rows*image->cols, sizeof(uchar));
				break;
			case CV_8S:
				image->data = calloc(image->rows*image->cols, sizeof(char));
				break;
			case CV_16U:
				image->data = calloc(image->rows*image->cols, sizeof(ushort));
				break;
			case CV_16S:
				image->data = calloc(image->rows*image->cols, sizeof(short));
				break;
			case CV_32S:
				image->data = calloc(image->rows*image->cols, sizeof(int));
				break;
			case CV_32F:
				image->data = calloc(image->rows*image->cols, sizeof(float));
				break;
			case CV_64F:
				image->data = calloc(image->rows*image->cols, sizeof(double));
				break;
			}

			//copy the image data
			for(int r = 1; r < image->rows - 1; r++)
			{
				for(int c = 1; c < image->cols - 1; c++)
				{
					switch(type)
					{
					case CV_8U:
						Data(image->data, image->cols, r, c, uchar) = Data(data, cols, r-1, c-1, uchar);
						break;
					case CV_8S:
						Data(image->data, image->cols, r, c, char) = Data(data, cols, r-1, c-1, char);
						break;
					case CV_16U:
						Data(image->data, image->cols, r, c, ushort) = Data(data, cols, r-1, c-1, ushort);
						break;
					case CV_16S:
						Data(image->data, image->cols, r, c, short) = Data(data, cols, r-1, c-1, short);
						break;
					case CV_32S:
						Data(image->data, image->cols, r, c, int) = Data(data, cols, r-1, c-1, int);
						break;
					case CV_32F:
						Data(image->data, image->cols, r, c, float) = Data(image->data, image->cols, r-1, c-1, float);
						break;
					case CV_64F:
						Data(image->data, image->cols, r, c, double) = Data(image->data, image->cols, r-1, c-1, double);
						break;
					}
				}
			}
			return image;
		}
	};

#endif