/*
 * Engine.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Engine.h"
#include "../common/Config.h"
#include "threephase/ThreephaseEngine.h"

namespace threescanner {

Engine* Engine::create(const Config& cfg) {
	auto type = cfg.get<std::string>("type");
	if (type == "threephase") {
		return new ThreephaseEngine(cfg);
	}
	return nullptr;
}

void Engine::destroy(Engine* obj) {
	delete obj;
}

Engine::Engine(const Config&) {

}

Engine::~Engine() {

}

} /* namespace threescanner */
