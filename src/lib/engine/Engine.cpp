/*
 * Engine.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Engine.h"
#include "../common/Config.h"
#include "threephase/ThreephaseEngine.h"

namespace threescanner {

Engine::Engine(const Config&) {

}

Engine::~Engine() {

}

void Engine::setParameter(const std::string& key, const std::string& value) {
	/* must be reimplemented by derived class */
}

} /* namespace threescanner */
