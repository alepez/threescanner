/*
 * CVImageInput.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_CVIMAGEINPUT_H_
#define threescanner_CVIMAGEINPUT_H_

#include "../ImageInput.h"

#include <thread>
#include <mutex>
#include <opencv2/highgui/highgui.hpp>

namespace threescanner {

class CVImageInput: public ImageInput {
public:
	CVImageInput(const Config& cfg);
	virtual ~CVImageInput();
	virtual ImagePtr grabImage();
	virtual void start();
	virtual void stop();
private:
	void configure(const Config&);
	void run();
	cv::VideoCapture* cap_;
	std::thread* thread_;
	volatile bool continueRunning_;
	std::mutex mutex_;
};

} /* namespace threescanner */

#endif /* threescanner_CVIMAGEINPUT_H_ */
