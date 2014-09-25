/*
 * Scanner.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_SCANNER_H_
#define threescanner_SCANNER_H_

#include "../prerequisites.h"
#include "../net/TcpServer.h"

#include <future>

namespace threescanner {

class Scanner: public TcpServer {
public:
	Scanner(const Config& cfg, EnginePtr engine = nullptr);
	virtual ~Scanner();
	std::future<void> start();
	void stop();
	void handleAction(const std::string& action, const std::vector<std::string>& params);
	void saveCloud(const std::string& filepath);
	void setEngine(EnginePtr);
private:
	void run();
	EnginePtr engine_;
	bool quit_;
};

} /* namespace threescanner */

#endif /* threescanner_SCANNER_H_ */
