#pragma once

class ImageProcessor {
	cv::Mat previous;
	cv::Mat variances;
	cv::Mat movement_hist;
	cv::Mat frame_buffer[4];
	int frame_number;
	int frame_buffer_index;
	int variance_index;
	int frame_height;
	int frame_width;
	float threshold;
	float average_variance;
	float surrounding_variances[6];
	bool initialised;
public:
	ImageProcessor(int, int);
	cv::Mat processFrame(cv::Mat, int);
	void updateFrameBuffer(cv::Mat);
private:
	void incrementFrameBufferIndex();
	void initialisePreviousFrame();
	void updatePreviousFrame(cv::Mat);
	void calculateVariances();
	void replaceFrameBuffer(cv::Mat);
	float getMean(float*);
	float getVariance(float*);
	float getVarianceBasedCoefficient(float);
	int getPreviousFrameBufferIndex();
	uint8_t updateMovementCounter(uint8_t, float);
	uint8_t averagePixelsTemporal(uint8_t, uint8_t, uint32_t);
};