/*
 * ImageInput.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "ImageInput.h"
#include "../common/Config.h"
#include "fs/FsImageInput.h"

namespace threescanner {

ImageInputPtr ImageInput::create(const std::string& type, const Config& cfg) {
	if (type == "fs") {
		return ImageInputPtr(new FsImageInput(cfg));
	}
	throw std::invalid_argument("Cannot intantiate ImageInput of type " + type);
}

ImageInputPtr ImageInput::create(const Config& cfg) {
	const std::string& type = cfg.get<std::string>("type");
	return ImageInput::create(type, cfg);
}

ImageInput::ImageInput(const Config& cfg) {

}

ImageInput::~ImageInput() {

}

} /* namespace threescanner */
