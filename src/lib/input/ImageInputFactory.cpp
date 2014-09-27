/*
 * ImageInputFactory.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "ImageInputFactory.h"
#include "../common/Config.h"
#include "fs/FsImageInput.h"

namespace threescanner {

ImageInputPtr ImageInputFactory::create(const std::string& type, const Config& cfg) {
	if (type == "fs") {
		return ImageInputPtr(new FsImageInput(cfg));
	}
	throw std::invalid_argument("Cannot intantiate ImageInput of type " + type);
}

ImageInputPtr ImageInputFactory::create(const Config& cfg) {
	const std::string& type = cfg.get<std::string>("type");
	return ImageInputFactory::create(type, cfg);
}

} /* namespace threescanner */
