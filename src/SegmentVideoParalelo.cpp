#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <omp.h>
#include "timer.h"

using namespace cv;
using namespace std;

template<typename T>
string ToString(T val) {
	stringstream stream;
	stream << val;
	return stream.str();
}

typedef struct MyImage {
   int w;
   int h;
} image;

int main(int argc, char **argv) {

	if (argc != 2) {
		std::cout << "USE: " << argv[0] << " <video-filename>" << std::endl;
		return 1;
	}

	//Open the video that you pass from the command line
	VideoCapture cap(argv[1]);
	if (!cap.isOpened()) {
		std::cerr << "ERROR: Could not open video " << argv[1] << std::endl;
		return 1;
	}
	
	int myRank;									// Rank of process
	int p =4;										// Number of processes
	MPI_Status status;							// Return status for receive

	// initializing MPI structures and checking p is odd
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	//Crea la estructura 
	int blocksCount = 2;
	int blocksLength[2] = {1, 1};

	MPI_Datatype types[2] = {MPI_INT, MPI_INT};
	MPI_Aint offsets[2];
	MPI_Datatype custom_type;
	offsets[0] = offsetof(image, w);
	offsets[1] = offsetof(image, h);

	MPI_Type_create_struct(blocksCount, blocksLength, offsets, types, &custom_type);
	MPI_Type_commit(&custom_type);
	

	int frame_count = 0;
	bool should_stop = false;

	int cantFrames = 0;
	
	while (!should_stop) {

		Mat frame;
		cap >> frame; //get a new frame from the video
		if (frame.empty()) {
			should_stop = true; //we arrived to the end of the video
			continue;
		}

		char filename[128];
		sprintf(filename, "Images/%d.jpg", frame_count);
		imwrite(filename, frame);
		frame_count++;
		cantFrames++;
	}

	printf("Total fotogramas: %d \n ", cantFrames);

    double elapsedTime;
	int h_bins = 50;
	int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };

	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };


	double *batchart = new double[cantFrames - 1];
    // starting timer
	timerStart();
	
	//*envia las imagenes
	if(myRank==0){
		for (int i = 0; i < cantFrames-1; i++) {
			//carga la imagen que va a enviar
			Mat src_images = imread("Images/" + ToString(i) + ".jpg",
					CV_LOAD_IMAGE_COLOR);
			
			image send;
			send.w = src_images.size().width;
			send.h = src_images.size().height;
			// Send image dimensions
			MPI_Send(&send, 1, custom_type, 1, 0, MPI_COMM_WORLD);
			// Send image data
			MPI_Send(src_images.data, send.w*send.h, MPI_UNSIGNED_CHAR, p%i, 0, MPI_COMM_WORLD);
		}
	}
	//Recive las imagenes
	else{
		Mat image_array[(cantFrames-1)/p];      // array of 10 images
		
		for (int i = 0; i < (cantFrames-1)/p; i++) {
			image recv;
			MPI_Status status;
			// Receive image dimensions
			MPI_Recv(&recv, 1, custom_type, 0, 0, MPI_COMM_WORLD, &status);
			// Allocate image matrix
			Mat mat(Size(recv.w, recv.h), CV_8UC3);
			// Receive image data
			MPI_Recv(mat.data, recv.w*recv.h, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
			
			image_array[i]= mat;
		}
		
	}
	
/*
	for (int i = 0; i < cantFrames-1; i++) {

		Mat src_picture1, hsv_picture1;
		Mat src_picture2, hsv_picture2;
		MatND hist_picture1;
		MatND hist_picture2;

		/// Load image
		src_picture1 = imread("Images/" + ToString(i) + ".jpg",
				CV_LOAD_IMAGE_COLOR);
		src_picture2 = imread("Images/" + ToString(i + 1) + ".jpg",
				CV_LOAD_IMAGE_COLOR);

		/// Convert to HSV
		cvtColor(src_picture1, hsv_picture1, COLOR_BGR2HSV);
		cvtColor(src_picture2, hsv_picture2, COLOR_BGR2HSV);

		/// Histograms
		/// Calculate the histograms for the HSV images
		calcHist(&hsv_picture1, 1, channels, Mat(), hist_picture1, 2, histSize,
				ranges, true, false);
		normalize(hist_picture1, hist_picture1, 0, 1, NORM_MINMAX, -1, Mat());
		calcHist(&hsv_picture2, 1, channels, Mat(), hist_picture2, 2, histSize,
				ranges, true, false);
		normalize(hist_picture2, hist_picture2, 0, 1, NORM_MINMAX, -1, Mat());

		double base = compareHist(hist_picture1, hist_picture2, 3);

		batchart[i] = base;

	}*/
	
	// stopping timer
	//elapsedTime = timerStop();

	//cout << elapsedTime << "\n";

	//Imprime el arreglo final
	/*for (int i = 0; i < cantFrames - 1; ++i) {
		double base = batchart[i];
		printf(" Foto %d con %d : %f \n", i, i + 1, base);
	}*/
	/*double promedio = batchart[0];
	for (int i = 1; i < cantFrames-1; ++i) {
		double base = batchart[i];
		//newj = newj >= 0 ? newj : 0;
		if (abs(base-promedio) >= 0.10) {
				printf(" Shot en foto %d con %f \n", i + 1, base);
				promedio = batchart[i+1];
			}
		else{promedio=base;}


	}
	*/
	//printf("Termine \n");

	return 0;
}
