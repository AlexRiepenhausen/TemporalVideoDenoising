#pragma once

class ImageProcessor {
	cv::Mat previous;
	cv::Mat vars;
	cv::Mat movement_hist;
	cv::Mat frame_buffer[4];
	int frame_number;
	int frame_buffer_index;
	int variance_index;
	int frame_height;
	int frame_width;
	float pixel_variance;
	float surrounding_variances[6];
	bool initialised;
public:
	ImageProcessor(int, int);
	cv::Mat processFrame(cv::Mat, int);
private:
	void updateFrameBuffer(cv::Mat);
	void incrementFrameBufferIndex();
	void initialisePreviousFrame(); 
	void calculateVariances();
	float getVariance(float*);
	float getMean(float*);
	float getMaxVarianceForPixel(int, int);
	float getVarianceBasedCoefficient(float);
	uint8_t updateMovementCounter(uint8_t, float);
	uint8_t averagePixelsTemporal(uint8_t, uint8_t, uint32_t);
	void updatePreviousFrame(cv::Mat);
	void replaceFrameBuffer(cv::Mat);
	int getPreviousFrameBufferIndex();
};