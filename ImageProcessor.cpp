#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <cmath>
#include "opencv2\opencv.hpp"
#include "ImageProcessor.h"


// constructor taking the denoising intensity or threshold value, together with height and width of video
// initialises frame of variance values, movement tracing frame and the previous frame
ImageProcessor::ImageProcessor(int height, int width) {
	frame_height       = height;
	frame_width        = width;
	variances          = cv::Mat::zeros(frame_height, frame_width * 3, CV_32FC1);
	previous           = cv::Mat::zeros(frame_height, frame_width, CV_8UC3);
	movement_hist      = cv::Mat::zeros(frame_height, frame_width, CV_8UC3);
}


// main function that takes a raw frame and returns a denoised frame
// requires four frames to be initialised, resulting in the first few frames 
// not getting denoised (can be fixed with additional programming)
// works for coloured video only, hence frame_width * 3 
cv::Mat ImageProcessor::processFrame(cv::Mat currentframe, int framenum) {

	frame_number = framenum;
	updateFrameBuffer(currentframe);

	if (initialised == true) {

		// create empty new frame and update four pixel variance values
		cv::Mat newframe = cv::Mat::zeros(frame_height, frame_width, CV_8UC3);
		calculateVariances();

		for (int i = 0; i < frame_height; i++) {
			for (int j = 0; j < frame_width * 3; j++) { 

				uint8_t previous_pixel = previous.at<uint8_t>(i, j);      // previous (already denoised) pixel
				uint8_t current_pixel  = currentframe.at<uint8_t>(i, j);  // raw pixel from current frame
				uint8_t movement_count = movement_hist.at<uint8_t>(i, j); // keeping track of how long pixel hasn't moved

				float variance         = variances.at<float>(i, j);              // get variance for current pixel
				float coefficient      = getVarianceBasedCoefficient(variance);  // get denoising coefficient

				// calculate movement count based on coefficient
				movement_count = updateMovementCounter(movement_count, coefficient);

				newframe.at<uint8_t>(i, j) = averagePixelsTemporal(previous_pixel, current_pixel, movement_count);

				// update movement history of this pixel
				movement_hist.at<uint8_t>(i, j) = movement_count;

			}
		}

		// set newly denoised frame as previous frame and 
		// replace current frame with denoised frame inside frame buffer
		updatePreviousFrame(newframe);
		replaceFrameBuffer(newframe);

		return newframe;

	}

	// only returns current frame when initialising
	return currentframe;

}



// get variance-based sigmoid coefficient
float ImageProcessor::getVarianceBasedCoefficient(float variance) {
	float coefficient = 1.0 / (1.0 + 4.0 * pow(0.8, variance - 12.0));
	return coefficient;
}


// use sigmoid function to adjust movement counter - no threshold needed
uint8_t ImageProcessor::updateMovementCounter(uint8_t movement, float coefficient) {
	uint8_t new_value = round( float(movement) * (1.0 - coefficient) ) + 1;
	return new_value;
}


// takes current and previous pixel value (colour channel value to be precise) and calculates the new average progressively
// based on the movement counter of said pixel
uint8_t ImageProcessor::averagePixelsTemporal(uint8_t previous, uint8_t current, uint32_t n) {
	float new_mean = (float)(current + (n - 1) * previous) / (float)n;
	return (uint8_t)new_mean;
}


// gets variance values for every pixel before denosing starts
void ImageProcessor::calculateVariances() {
	for (int i = 0; i < frame_height; i++) {
		for (int j = 0; j < frame_width * 3; j++) {
			float temp[4];
			for (int k = 0; k < 4; k++) {
				temp[k] = frame_buffer[k].at<uint8_t>(i, j);
			}
			variances.at<float>(i, j) = getVariance(temp);
		}
	}
}


// helper function to calculate mean from array
float ImageProcessor::getMean(float* data) {
	float sum = 0.0;
	for (size_t i = 0; i < 4; i++)
		sum += data[i];
	return sum / 4.0;
}


// helper function to calculate variance from array
float ImageProcessor::getVariance(float* data) {
	float mean = getMean(data);
	float temp = 0;
	for (size_t i = 0; i < 4; i++)
		temp += (data[i] - mean)*(data[i] - mean);
	return temp / 4.0;
}


// takes current frame and puts it into the frame buffer queue
void ImageProcessor::updateFrameBuffer(cv::Mat frame) {
	frame_buffer[frame_buffer_index] = frame;
	incrementFrameBufferIndex();
}


// replaces raw frame with denoised frame in the frame buffer queue
// the index has to be decremented since it already had been updated previously
void ImageProcessor::replaceFrameBuffer(cv::Mat frame) {
	int previous_index = getPreviousFrameBufferIndex();
	frame_buffer[previous_index] = frame;
}


// get the preceding frame buffer index
void ImageProcessor::incrementFrameBufferIndex() {
	frame_buffer_index += 1;
	if (frame_buffer_index > 3) {
		if (initialised == false) {
			initialisePreviousFrame();
			initialised = true;
		}
		frame_buffer_index = 0;
	}
}


// get the preceding frame buffer index
int ImageProcessor::getPreviousFrameBufferIndex() {
	if (frame_buffer_index == 0) {
		return 3;
	}
	return frame_buffer_index - 1;
}


// intitialises previous frame from frame buffer - happens once before denoising starts
void ImageProcessor::initialisePreviousFrame() {
	for (int i = 0; i < frame_height; i++) {
		for (int j = 0; j < frame_width * 3; j++) {
			previous.at<uint8_t>(i, j) = frame_buffer[frame_buffer_index-2].at<uint8_t>(i, j);
		}
	}
}


// overwrites the previous frame with whatever frame you specifiy
void ImageProcessor::updatePreviousFrame(cv::Mat current) {
	for (int i = 0; i < frame_height; i++) {
		for (int j = 0; j < frame_width * 3; j++) {
			previous.at<uint8_t>(i, j) = current.at<uint8_t>(i, j);
		}
	}
}
