/*
 * Engine.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Engine.h"
#include "../common/Config.h"
#include "threephase/ThreephaseEngine.h"

namespace threescanner {

Engine::Engine(const Config& cfg, ImageInput* input) :
				input_(input) {

}

Engine::~Engine() {

}

} /* namespace threescanner */
