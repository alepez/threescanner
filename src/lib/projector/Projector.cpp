/*
 * Projector.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Projector.h"

namespace threescanner {

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
