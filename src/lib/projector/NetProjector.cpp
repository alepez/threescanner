/*
 * NetProjector.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "NetProjector.h"

#include "../common/Config.h"

namespace threescanner {

NetProjector::NetProjector(const Config& cfg) :
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

} /* namespace threescanner */
