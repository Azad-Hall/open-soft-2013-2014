#ifndef RESIZE_HPP
#define RESIZE_HPP 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;
static const double PI = 3.14159265359;

namespace Resize
{
  Mat resizeImage(Mat src, Size size) {
    Mat res;
    // src = src.clone();
    // GaussianBlur(src, src, Size(3, 3), 0, 0);
    resize(src, res, size, 0, 0, INTER_LINEAR); /// should generally be used only for downsizing!
    // normalize(res, res, 0, 255, CV_MINMAX);
    return res;
  }

  Mat rotateTranslateImage(Mat src, float deg, Point rotPoint, double dx, double dy) {
    assert(src.cols != 0 && src.rows != 0);
    /// Calculate roi
    RotatedRect rr = RotatedRect(Point(0,0), Size(src.cols, src.rows), deg);
    int newWidth = rr.boundingRect().width;
    int newHeight = rr.boundingRect().height;

    double angle = deg;
    double scale = 1.0;

    Mat rot_mat = getRotationMatrix2D( rotPoint, angle, scale );
    rot_mat.at<double>(0,2) += dx;
    rot_mat.at<double>(1,2) += dy;
    Mat result;
    warpAffine( src, result, rot_mat, Size(src.cols, src.rows) );
    return result;
  }

  Mat unrotateImage(Mat src, float deg, Size origSize) {
     assert(src.cols != 0 && src.rows != 0);
     
     Point center = Point( src.cols/2, src.rows/2 );
     double angle = deg;
     double scale = 1.0;
     
     Mat rot_mat = getRotationMatrix2D( center, angle, scale );
     rot_mat.at<double>(0,2) += (origSize.width-src.cols)/2;
     rot_mat.at<double>(1,2) += (origSize.height-src.rows)/2;
     Mat result;
     warpAffine( src, result, rot_mat, origSize );
     return result;
  }
  void transformSet(Mat matrix[], double dx[], double dy[], double deg[], Point points[], int nElm) {
    assert(nElm > 0);
    Mat rot = getRotationMatrix2D(Point(0,0), 0, 1);
    rot.at<double>(0,2) += matrix[0].cols;
    rot.at<double>(1,2) += matrix[0].rows;
    double offsetX = matrix[0].cols;
    double offsetY = matrix[0].rows;
    warpAffine(matrix[0], matrix[0], rot, Size(matrix[0].cols*3, matrix[0].rows*3));
    for(int i = 1; i < nElm; i++) {
      rot = getRotationMatrix2D(Point(0,0), 0, 1);
      warpAffine(matrix[i], matrix[i], rot, Size(matrix[0].cols, matrix[0].rows));
      // dx[i] += offsetX;
      // dy[i] += offsetY;
      matrix[i] = rotateTranslateImage(matrix[i], deg[i], points[i], dx[i] + offsetX, dy[i] + offsetY);
    }
  }
  Mat mergeMasks(Mat matrix[], int nElm) { // note: transformSet shoukld have already been called on this matrix set
    Mat res = matrix[0].clone();
    for(int i = 1; i < nElm; i++) {
      assert(matrix[i].cols <= res.cols && matrix[i].rows <= res.rows);
      for (int r = 0; r < matrix[i].rows; ++r)
      {
        for (int c = 0; c < matrix[i].cols; ++c)
        {
          if(matrix[i].at<char>(r, c) != 0) {
            res.at<char>(r, c) = matrix[i].at<char>(r, c);
          }
        }
      }
    }
    return res;
  }
  Mat doIt(Mat images[], Mat masks[], double dx[], double dy[], double deg[], Point points[], int nElm) {
    // transformSet(images, dx, dy, deg, points, nElm);
    transformSet(masks, dx, dy, deg, points, nElm);
    printf("mask sizes:\n");    
    for (int i = 0; i < nElm; ++i)
    {
      printf("[%d,%d], ", masks[i].cols, masks[i].rows);
    }
    printf("\n");
    Mat finalMask = mergeMasks(masks, nElm);
    return finalMask;
    Mat res = images[0].clone();
    for(int i = 1; i < nElm; i++) {
      assert(images[i].cols <= res.cols && images[i].rows <= res.rows);
      assert(images[i].cols == masks[i].cols && images[i].rows == masks[i].rows);
      for (int r = 0; r < images[i].rows; ++r)
      {
        for (int c = 0; c < images[i].cols; ++c)
        {
          if(masks[i].at<char>(r, c) != 0) {
            res.at<char>(r, c) = images[i].at<char>(r, c);
          }
        }
      }
    }
    return res;
  }
  Mat cutRotatedRect(Mat src, RotatedRect rr) {
    Mat result;
    Mat rot_mat = getRotationMatrix2D(rr.center, -rr.angle, 1.0);
    Size newSize = rr.size;
    rot_mat.at<double>(0,2) += (newSize.width-src.cols)/2;
    rot_mat.at<double>(1,2) += (newSize.height-src.rows)/2;
    warpAffine(src, result, rot_mat, newSize);
    return result;
  }
}
#endif


Mat mergeAndSave(double dx[], double dy[], double deg[], Point points[], int nElm)
{
  // Mat img1 = imread(argv[1], 0);
  // resize(img1, img1, Size(img1.cols/3, img1.rows/3));
  // double angle = atof(argv[2]);
  // Point p;
  // p.x = atof(argv[3]);
  // p.y = atof(argv[4]);
  // // img1 = Resize::rotateTranslateImage(img1, angle, p, 100, 200);
  // Mat res;
  // Mat rot = getRotationMatrix2D(Point(0,0), 0, 1);
  // rot.at<double>(0,2) += img1.cols;
  // rot.at<double>(1,2) += img1.rows;
  // warpAffine(img1, img1, rot, Size(img1.cols*3, img1.rows*3));
  // namedWindow("1");
  // imshow("1", img1);
  // waitKey();
  Mat images[100], masks[100];
  // double dx[3] = {0, -0, -0}, dy[3] = {0, 0, 0}, deg[3] = {0, 0.121614, 0.008924};
  // Point points[3] = {Point(0,0), Point(1204,2148), Point(788,1273)};
  double scaleFactor = 3;
  for (int i = 0; i < nElm; ++i)
  {
    deg[i] = deg[i]*180/3.141;
    dx[i] /= scaleFactor;
    dy[i] /= scaleFactor;
    points[i].x /= scaleFactor;
    points[i].y /= scaleFactor;
  }
  for (int i = 0; i < nElm; ++i)
  {
    char buf[100];
    sprintf(buf, "Maskedimage%d.jpg", i);
    images[i] = imread(buf, 0);
    resize(images[i], images[i], Size(images[i].cols/scaleFactor, images[i].rows/scaleFactor));
    sprintf(buf, "Maskedimage%d.jpg", i);
    masks[i] = imread(buf, 0);
    resize(masks[i], masks[i], Size(masks[i].cols/scaleFactor, masks[i].rows/scaleFactor));
  }
  Mat res = Resize::doIt(images, masks, dx, dy, deg, points, nElm);
  return res;
}