/*
 * Projector.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Projector.h"
#include "../common/Config.h"
#include "threephase/ThreephaseProjector.h"

namespace threescanner {

Projector* Projector::create(const Config& cfg) {
	auto type = cfg.get<std::string>("type");
	if (type == "threephase") {
		return new ThreephaseProjector(cfg);
	}
	return nullptr;
}

void Projector::destroy(Projector*) {

}

Projector::Projector(const Config&) {

}

Projector::~Projector() {

}

} /* namespace threescanner */
