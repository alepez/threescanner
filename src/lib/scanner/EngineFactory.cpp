/*
 * EngineFactory.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "EngineFactory.h"
#include "../common/Config.h"
#include "threephase/ThreephaseEngine.h"

namespace threescanner {

EnginePtr EngineFactory::create(const std::string& type, const Config& cfg) {
	if (type == "threephase") {
		return EnginePtr(new ThreephaseEngine(cfg));
	}
	throw std::invalid_argument("Cannot intantiate Engine of type " + type);
}

EnginePtr EngineFactory::create(const Config& cfg) {
	const std::string& type = cfg.get<std::string>("type");
	return EngineFactory::create(type, cfg);
}

} /* namespace threescanner */
