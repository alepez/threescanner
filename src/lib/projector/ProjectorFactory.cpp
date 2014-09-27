/*
 * ProjectorFactory.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "ProjectorFactory.h"
#include "../common/Config.h"
#include "threephase/ThreephaseProjector.h"
#include "NetProjector.h"

namespace threescanner {

ProjectorPtr ProjectorFactory::create(const std::string& type, const Config& cfg) {
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

ProjectorPtr ProjectorFactory::create(const Config& cfg) {
	const std::string& type = cfg.get<std::string>("type");
	return ProjectorFactory::create(type, cfg);
}

} /* namespace threescanner */
