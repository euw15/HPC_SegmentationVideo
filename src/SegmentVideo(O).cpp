//============================================================================
// Name        : SegmentVideo.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <cv.h>
#include <string>
#include <cmath>
#include <omp.h>

using namespace cv;
using namespace std;

template <typename T>
	string ToString(T val)
	{
	    stringstream stream;
	    stream << val;
	    return stream.str();
	}
/**
 * @function main
 */
int main(int argc, char** argv) {

	/// Using 50 bins for hue and 60 for saturation (Ver explicacion en
	//http://stackoverflow.com/questions/24725224/how-to-choose-the-number-of-bins-when-creating-hsv-histogram)
	int h_bins = 50;
	int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };

	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };

	int cantFrames=11;
	double *batchart = new double[cantFrames-1];


    
	for (int i = 0; i < cantFrames-1; i++) {

			 Mat src_picture1, hsv_picture1;
			 Mat src_picture2, hsv_picture2;
			 MatND hist_picture1;
			 MatND hist_picture2;

			 /// Load image
			 src_picture1 = imread(ToString(i)+".jpg", CV_LOAD_IMAGE_COLOR);
			 src_picture2 = imread(ToString(i+1)+".jpg", CV_LOAD_IMAGE_COLOR);

			 /// Convert to HSV
			 cvtColor(src_picture1, hsv_picture1, COLOR_BGR2HSV);
			 cvtColor(src_picture2, hsv_picture2, COLOR_BGR2HSV);

			 /// Histograms
			 /// Calculate the histograms for the HSV images
			 calcHist(&hsv_picture1, 1, channels, Mat(), hist_picture1, 2, histSize, ranges,
			 			true, false);
			 normalize(hist_picture1, hist_picture1, 0, 1, NORM_MINMAX, -1, Mat());
             calcHist(&hsv_picture2, 1, channels, Mat(), hist_picture2, 2, histSize, ranges,
			 			true, false);
			 normalize(hist_picture2, hist_picture2, 0, 1, NORM_MINMAX, -1, Mat());

			 double base = compareHist(hist_picture1, hist_picture2, 3);

			 batchart[i]=base;
			 /*imwrite("FilterImage.jpg", src_picture1);
			 waitKey(0);*/


		 }

	//Imprime el arreglo final
	for(int i = 0; i < cantFrames-1; ++i){
		double base =batchart[i];
		printf(" Foto %d con %d : %f \n",i,i+1,base);
	}

	double promedio = batchart[0];
	for (int i = 1; i < cantFrames-1; ++i) {
		double base = batchart[i];
		//newj = newj >= 0 ? newj : 0;
		if (abs(base-promedio) >= 0.10) {
				printf(" Shot en foto %d con %f \n", i + 1, base);
				promedio = batchart[i+1];
			}
		else{promedio=base;}


	}
	return 0;

}
