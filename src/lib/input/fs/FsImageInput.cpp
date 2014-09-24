/*
 * FsImageInput.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "FsImageInput.h"
#include "../../common/Config.h"

#include <opencv2/highgui/highgui.hpp>

namespace threescanner {

FsImageInput::FsImageInput(const Config& cfg) :
				ImageInput(cfg),
				images_(),
				index_(0) {
	auto path = cfg.get<std::string>("path");
	auto files = cfg.getChild("files").getAll<std::string>();
	for (auto file: files) {
		auto filepath = path + "/" + file.second;
		cv::Mat image = cv::imread(filepath);
		images_.push_back(ImagePtr(new Image(image)));
	}
}

FsImageInput::~FsImageInput() {

}

ImagePtr FsImageInput::grabImage() {
	ImagePtr ret = images_[index_];
	index_ = (index_ + 1) % images_.size();
	return ret;
}

} /* namespace threescanner */
