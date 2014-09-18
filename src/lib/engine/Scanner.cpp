/*
 * Scanner.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Scanner.h"
#include "../common/Config.h"
#include "../common/Logger.h"
#include "Engine.h"

#include <boost/algorithm/string.hpp>

namespace threescanner {

Scanner::Scanner(const Config& cfg, Engine* engine) :
				TcpServer(cfg.getChild("net")),
				engine_(engine) {
}

Scanner::~Scanner() {

}

void Scanner::run(const bool& continueRunning) {
	while (continueRunning) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		this->syncComm();
	}
}

void Scanner::handleAction(const std::string& action, const std::vector<std::string>& params) {
	if (action.empty()) {
		return;
	}
	logTrace(action + " => " + boost::join(params, " "));
	if ((action == "set") && params.size() == 2) {
		engine_->setParameter(params[0], params[1]);
		return;
	}
	logWarning("Projector: unknown action: " + action);
}

} /* namespace threescanner */
