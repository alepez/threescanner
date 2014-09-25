/*
 * TcpClient.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_TCPCLIENT_H_
#define threescanner_TCPCLIENT_H_

#include "../prerequisites.h"
#include "TcpBase.h"

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <queue>

namespace threescanner {

class TcpClient: public TcpBase {
public:
	TcpClient(const Config& cfg);
	virtual ~TcpClient();
private:
	void runTcpClient();
	/* rete */
	boost::asio::ip::tcp::endpoint endpoint_;
	/* thread */
	volatile bool continueRunning_;
	boost::thread* thread_;
	bool ignoreErrors_;
};

}

#endif /* threescanner_TCPCLIENT_H_ */
