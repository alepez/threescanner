/*
 * Scanner.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Scanner.h"
#include "../common/Config.h"
#include "../common/Logger.h"
#include "Engine.h"

#include <pcl/io/pcd_io.h>

namespace threescanner {

Scanner::Scanner(const Config& cfg, EnginePtr engine) :
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
	if (action == "save") {
		std::string filepath = params.size() > 0 ? params[0] : "output.pcd";
		this->saveCloud(filepath);
		return;
	}
	if ((action == "set") && params.size() == 2) {
		engine_->setParameter(params[0], params[1]);
		return;
	}
	logWarning("Projector: unknown action: " + action);
}

void Scanner::saveCloud(const std::string& filepath) {
	try {
		pcl::io::savePCDFile(filepath, *engine_->getCloud());
		logInfo("PointCloud saved to " + filepath);
	} catch (const std::exception& e) {
		logError("Cannot save PointCloud to " + filepath + ": " + e.what());
	}
}

void Scanner::setEngine(EnginePtr engine) {
	engine_ = engine;
}

} /* namespace threescanner */
