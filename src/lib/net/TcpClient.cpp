/*
 * TcpClient.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "TcpClient.h"
#include "../common/Logger.h"
#include "../common/Config.h"

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <stdexcept>

using namespace std;
using namespace boost;

using boost::asio::ip::tcp;

namespace threescanner {

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

TcpClient::TcpClient(const Config& cfg) :
				TcpBase(cfg),
				continueRunning_(true),
				ignoreErrors_(cfg.get<bool>("ignoreErrors")) {
	try {
		auto hostname = cfg.get<std::string>("host");
		auto port = cfg.get<int>("port");
		logDebug("Connecting to %s:%i (%s)", hostname.c_str(), port, name_.c_str());
		endpoint_ = tcp::endpoint(asio::ip::address::from_string(hostname), port);
	} catch (const std::exception& e) {
		throw runtime_error(string("Cannot initialize TcpClient: ") + e.what());
	}
	thread_ = new boost::thread(boost::bind(&TcpClient::run, this));
}

TcpClient::~TcpClient() {
	continueRunning_ = false;
	thread_->join();
	delete thread_;
}

void TcpClient::run() {
	logTrace(name_ + " TcpClient start");
	while (continueRunning_) {
		try {
			boost::asio::io_service ioService;
			tcp::socket s(ioService);
			tcp::resolver resolver(ioService);
			boost::asio::connect(s, resolver.resolve(endpoint_));
			try {
				while (continueRunning_) {
					this->update(s);
					boost::this_thread::sleep(boost::posix_time::milliseconds(1));
				}
			} catch (const boost::system::system_error& e) {
				if (e.code() == boost::asio::error::eof) {
					logWarning("Connection closed");
					break;
				} else {
					throw;
				}
			} catch (const std::exception& e) {
				logWarning(e.what());
			}
		} catch (const std::exception& e) {
			if (!ignoreErrors_) {
				throw;
			}
			logWarning(e.what());
			boost::this_thread::sleep(boost::posix_time::seconds(10));
		}
	}
	logTrace(name_ + " TcpClient stop");
}

}
