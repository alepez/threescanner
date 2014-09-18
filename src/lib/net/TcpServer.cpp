/*
 * TcpServer.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "TcpServer.h"
#include "../common/Logger.h"
#include "../common/Config.h"

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace std;
using namespace boost;

using boost::asio::ip::tcp;

namespace threescanner {

TcpServer::TcpServer(const Config& cfg) :
				TcpBase(cfg),
				ioService_(),
				acceptor_(ioService_, tcp::endpoint(tcp::v4(), cfg.get<int>("port"))),
				continueRunning_(true) {
	thread_ = new boost::thread(boost::bind(&TcpServer::run, this));
}

TcpServer::~TcpServer() {
	continueRunning_ = false;
	thread_->join();
	delete thread_;
}

void TcpServer::listen() {
	socket_ = socket_ptr(new tcp::socket(ioService_));
	acceptor_.async_accept(*socket_, [this](boost::system::error_code error) {
		if (!error) {
			this->session(socket_);
		}
	});
}

void TcpServer::run() {
	this->listen();
	while (continueRunning_) {
		/* poll invece di run evita che si blocchi qui, cosi` posso
		 * chiudere il server anche da esterno
		 */
		ioService_.poll();
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	}
}

void TcpServer::session(socket_ptr s) {
	logTrace(name_ + " TcpServer new session");
	try {
		while (continueRunning_) {
			this->update(*s);
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}
	} catch (const boost::system::system_error& e) {
		if (e.code() == boost::asio::error::eof) {
			logWarning("Connection closed");
			this->listen();
		} else {
			throw;
		}
	} catch (const std::exception& e) {
		logWarning(e.what());
	}
}

}
