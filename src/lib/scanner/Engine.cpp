/*
 * Engine.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Engine.h"
#include "../common/Config.h"
#include "threephase/ThreephaseEngine.h"

namespace threescanner {

EnginePtr Engine::create(const std::string& type, const Config& cfg) {
	if (type == "threephase") {
		return EnginePtr(new ThreephaseEngine(cfg));
	}
	throw std::invalid_argument("Cannot intantiate Engine of type " + type);
}

EnginePtr Engine::create(const Config& cfg) {
	const std::string& type = cfg.get<std::string>("type");
	return Engine::create(type, cfg);
}

Engine::Engine(const Config&) :
				input_(nullptr),
				cloud_(new PointCloud()) {

}

Engine::~Engine() {

}

void Engine::setInput(ImageInputPtr input) {
	if (input_.get() != nullptr) {
		throw std::invalid_argument("Engine: cannot set input if already defined.");
	}
	input_ = input;
}

PointCloud::ConstPtr Engine::getCloud() const {
	return cloud_;
}

} /* namespace threescanner */
