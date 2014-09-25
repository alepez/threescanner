/*
 * NetProjector.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "NetProjector.h"

#include "../common/Config.h"

namespace threescanner {

NetProjector::NetProjector(const Config& cfg) :
				Projector("net", cfg),
				TcpClient(cfg.getChild("net")) {

}

NetProjector::~NetProjector() {

}

void NetProjector::setParameter(const std::string& key, const std::string& value) {
	this->sendAction(key, { value });
}

void NetProjector::quit() {
	this->sendAction("quit");
}

std::future<void> NetProjector::start() {
	return std::future<void>();
}

void NetProjector::stop() {

}

} /* namespace threescanner */
