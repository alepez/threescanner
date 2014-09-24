/*
 * Engine.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Engine.h"
#include "../common/Config.h"
#include "threephase/ThreephaseEngine.h"

namespace threescanner {

EnginePtr Engine::create(const std::string& type, const Config& cfg, ImageInput* input) {
	if (type == "threephase") {
		return EnginePtr(new ThreephaseEngine(cfg, input));
	}
	throw std::invalid_argument("Cannot intantiate Engine of type " + type);
}

EnginePtr Engine::create(const Config& cfg, ImageInput* input) {
	const std::string& type = cfg.get<std::string>("type");
	return Engine::create(type, cfg, input);
}

//static const unsigned MAX_POINTS = (1024 * 1024); TODO: optimization

Engine::Engine(const Config&, ImageInput* input) :
				input_(input),
				cloud_(new PointCloud()) {
//	cloud_->reserve(MAX_POINTS); TODO
}

Engine::~Engine() {

}

PointCloud::ConstPtr Engine::getCloud() const {
	return cloud_;
}

} /* namespace threescanner */
