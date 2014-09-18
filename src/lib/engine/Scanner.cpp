/*
 * Scanner.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Scanner.h"
#include "../common/Config.h"
#include "../common/Logger.h"
#include "Engine.h"

namespace threescanner {

Scanner::Scanner(const Config& cfg, Engine* engine) :
				TcpServer(cfg.getChild("net")),
				engine_(engine),
				quit_(false) {
}

Scanner::~Scanner() {

}

void Scanner::run(const bool& continueRunning) {
	while (continueRunning && !quit_) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		this->syncComm();
	}
}

void Scanner::handleAction(const std::string& action, const std::vector<std::string>& params) {
	if (action.empty()) {
		return;
	}
	if (action == "quit") {
		quit_ = true;
		return;
	}
	if (action == "scan") {
		engine_->startScan();
		return;
	}
	if ((action == "set") && params.size() == 2) {
		engine_->setParameter(params[0], params[1]);
		return;
	}
	logWarning("Projector: unknown action: " + action);
}

} /* namespace threescanner */
