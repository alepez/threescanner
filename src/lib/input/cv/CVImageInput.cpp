/*
 * CVImageInput.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "CVImageInput.h"
#include "../../common/Config.h"

#include <opencv2/highgui/highgui.hpp>

namespace threescanner {

CVImageInput::CVImageInput(const Config& cfg) :
				ImageInput(cfg),
				cap_(nullptr),
				thread_(nullptr),
				continueRunning_(true),
				mutex_() {

	cap_ = new cv::VideoCapture(cfg.get<int>("device"));
	if (!cap_->isOpened()) {
		throw std::runtime_error("Cannot open camera device");
	}
	auto format = cfg.get<std::string>("format");
	if (format == "mjpeg") {
		cap_->set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
	} else {
		throw std::runtime_error("Camera invalid format: " + format);
	}

	cap_->set(CV_CAP_PROP_FRAME_WIDTH, cfg.get<double>("width"));
	cap_->set(CV_CAP_PROP_FRAME_HEIGHT, cfg.get<double>("height"));
}

CVImageInput::~CVImageInput() {

}

ImagePtr CVImageInput::grabImage() {
	ImagePtr ret(new Image);
	mutex_.lock();
	cap_->retrieve(*ret);
	mutex_.unlock();
	return ret;
}

void CVImageInput::start() {
	continueRunning_ = true;
	thread_ = new std::thread(std::bind(&CVImageInput::run, this));
}

void CVImageInput::stop() {
	continueRunning_ = false;
	thread_->join();
	delete thread_;
}

void CVImageInput::run() {
	while (continueRunning_) {
		mutex_.lock();
		bool success = cap_->grab();
		mutex_.unlock();
		if (!success) {
			throw std::runtime_error("Cannot read a frame from video stream");
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

} /* namespace threescanner */
