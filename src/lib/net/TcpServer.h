/*
 * TcpServer.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_TCPSERVER_H_
#define threescanner_TCPSERVER_H_

#include "../prerequisites.h"
#include "TcpBase.h"

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <queue>

namespace threescanner {

class TcpServer: public TcpBase {
public:
	TcpServer(const Config& cfg);
	virtual ~TcpServer();
private:
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
	void runTcpServer();
	void session(socket_ptr sock);
	void listen();
	boost::asio::io_service ioService_;
	boost::asio::ip::tcp::acceptor acceptor_;
	volatile bool continueRunning_;
	boost::thread* thread_;
	socket_ptr socket_;
};

}

#endif /* threescanner_TCPSERVER_H_ */
