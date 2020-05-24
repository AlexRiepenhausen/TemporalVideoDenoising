#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>
#include <cmath>
#include "opencv2\opencv.hpp"
#include <opencv2\videoio.hpp>
#include "ImageProcessor.h"


// helper function to put a progress bar into the command line
void printProgressBar(int framecount, int total_num_frames) {

	double progress = (double)framecount/(double)total_num_frames;
	int barwidth    = 100;
	int pos         = barwidth * progress;

	std::cout << "[";
	for (int i = 0; i < barwidth; ++i) {
		if (i < pos) std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}

	std::cout << "] " << int(progress * 100.0) << " %\r";
	std::cout.flush();

}


// get input from command line for video file name
std::string getInputFileName(){
	std::string inputFile;
	std::cout << "Enter input file name : ";
	std::cin >> inputFile;
	std::cout << std::endl;
	return inputFile;
}


// get input from command line for output file name
std::string getOutputFileName(std::string input_file_name) {
	std::string  output_file_name = input_file_name.substr(0, input_file_name.size() - 4) + "_denoised.avi";
	std::cout << "Output video: " + output_file_name << std::endl;
	return output_file_name;
}


int main() {

	// read input and output file name and denoising intensity from command line
	std::string input_file_name  = getInputFileName();
	std::string output_file_name = getOutputFileName(input_file_name);

	// set video and denoising intensity - recommended between 8.0 and 25.0 depending on noise level
	cv::VideoCapture cap(input_file_name);

	// get frame width, height and frames per second
	int frame_width  = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	double fps       = cap.get(cv::CAP_PROP_FPS);

	// show error if file can't be opened
	if (!cap.isOpened()) {
		std::cout << "Error opening video stream or file" << std::endl;
		return -1;
	}

	// create video writer object and initialise processing unit
	ImageProcessor proc(frame_height, frame_width);
	cv::VideoWriter video(output_file_name, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(frame_width, frame_height));
	video.set(cv::VIDEOWRITER_PROP_QUALITY, 1.0);

	// iterate over every single frame in the video
	int framecount = 1;

	while (1) {

		cv::Mat frame;

		cap >> frame;

		if (frame.empty())
			break;

		cv::Mat newframe = proc.processFrame(frame, framecount);

		video.write(newframe);

		framecount +=1;
		printProgressBar(framecount, cap.get(7));

	}

	// release the video capture and writer object
	cap.release();
	video.release();

	// Closes all the frames
	cv::destroyAllWindows();

	return 0;

}