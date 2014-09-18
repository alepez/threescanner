/*
 * Scanner.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_SCANNER_H_
#define threescanner_SCANNER_H_

#include "../prerequisites.h"
#include "../net/TcpServer.h"

namespace threescanner {

class Scanner: public TcpServer {
public:
	Scanner(const Config& cfg, Engine* engine);
	virtual ~Scanner();
	void run(const bool& continueRunning);
private:
	void handleAction(const std::string& action, const std::vector<std::string>& params);
	void saveCloud(const std::string& filepath);
	Engine* engine_;
	bool quit_;
};

} /* namespace threescanner */

#endif /* threescanner_SCANNER_H_ */
