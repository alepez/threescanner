/*
 * CVImageInput.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "CVImageInput.h"
#include "../../common/Config.h"

#include <format.h>
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
		throw std::invalid_argument("Camera invalid format: " + format);
	}

	cap_->set(CV_CAP_PROP_FRAME_WIDTH, cfg.get<double>("width"));
	cap_->set(CV_CAP_PROP_FRAME_HEIGHT, cfg.get<double>("height"));

	this->configure(cfg);
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

void CVImageInput::configure(const Config& cfg) {
	/*
	 * FIXME: VERY HACKY
	 * Camera options should be handled by cv::VideoCapture but in
	 * opencv 2.4.9 almost all options aren't supported.
	 * We use `v4l2-ctl` to configure the device.
	 */
	auto options = cfg.getChild("options").getAll<int>();
	std::stringstream ss;
	ss << "v4l2-ctl ";
	ss << fmt::sprintf("-d /dev/video%i", cfg.get<int>("device"));
	for (auto& opt : options) {
		ss << " -c ";
		ss << opt.first;
		ss << "=";
		ss << opt.second;
		ss << " ";
	}
	const std::string& cmd = ss.str();
	if (::system(cmd.c_str())) {
		throw std::runtime_error("Cannot set camera parameters");
	}
}

} /* namespace threescanner */
