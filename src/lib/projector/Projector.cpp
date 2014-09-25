/*
 * Projector.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Projector.h"
#include "../common/Config.h"
#include "../common/Logger.h"
#include "Quad.h"
#include "Shaders.h"

#include "threephase/ThreephaseProjector.h"
#include "NetProjector.h"

#include <GLFW/glfw3.h>
#include <boost/algorithm/string.hpp>

namespace threescanner {

ProjectorPtr Projector::create(const std::string& type, const Config& cfg) {
	if (type == "net") {
		return ProjectorPtr(new NetProjector(cfg));
	}
	if (type == "threephase") {
		return ProjectorPtr(new ThreephaseProjector(cfg));
	}
	/*
	 * ADD HERE OTHER IMPLEMENTATIONS
	 */
	throw std::invalid_argument("Cannot intantiate Projector of type " + type);
}

ProjectorPtr Projector::create(const Config& cfg) {
	const std::string& type = cfg.get<std::string>("type");
	return Projector::create(type, cfg);
}

Projector::Projector(const std::string& type, const Config&) :
				engineType_(type) {

}

Projector::~Projector() {

}

void Projector::waitUntilReady() const {
	while (!this->isReady()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

} /* namespace threescanner */
