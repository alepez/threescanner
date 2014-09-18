/*
 * Engine.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Engine.h"
#include "../common/Config.h"
#include "threephase/ThreephaseEngine.h"

namespace threescanner {

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
