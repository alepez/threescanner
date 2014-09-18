/*
 * TcpBase.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "TcpBase.h"
#include "../common/Logger.h"
#include "../common/Config.h"

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

namespace threescanner {

static const int MAX_LEN = 1024;

TcpBase::TcpBase(const Config& cfg) :
				name_(cfg.get<std::string>("name")) {

}

TcpBase::~TcpBase() {

}

void TcpBase::sendMessage(const std::string& message) {
	mutex::scoped_lock lock(mutex_);
	outQueue_.push(message + "\n");
}

void TcpBase::sendQueuedMessages(tcp::socket& s) {
	mutex::scoped_lock lock(mutex_);
	while (!outQueue_.empty()) {
		std::string message = outQueue_.front();
		boost::system::error_code error;
		s.write_some(boost::asio::buffer(message.data(), message.size()), error);
		if (error) {
			throw boost::system::system_error(error);
		}
		logTrace("TX: " + message);
		outQueue_.pop();
	}
}

void TcpBase::receiveMessages(tcp::socket& s) {
	mutex::scoped_lock lock(mutex_);
	boost::system::error_code error;
	char data[MAX_LEN];
	size_t length = s.read_some(asio::buffer(data), error);
	if (length == 0 && error == asio::error::would_block) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		return;
	}
	if (error) {
		throw boost::system::system_error(error);
	}
	data[length] = '\0';
	logTrace(name_ + " TcpServer new message");
	std::string messagesStr(data);
	std::vector<std::string> messages;
	split(messages, messagesStr, is_any_of("\r\n"));
	for_each(messages.begin(), messages.end(), [this](std::string& message) {
		logTrace("RX: " + message);
		inQueue_.push(message);
	});
}

bool TcpBase::getNextMessage(std::string* message_out) {
	mutex::scoped_lock lock(mutex_, try_to_lock);
	if (!lock) {
		return false;
	}
	if (inQueue_.empty()) {
		message_out->clear();
		return false;
	} else {
		*message_out = inQueue_.front();
		inQueue_.pop();
		return true;
	}
}

void TcpBase::update(boost::asio::ip::tcp::socket& s) {
	s.non_blocking(true);
	this->sendQueuedMessages(s);
	this->receiveMessages(s);
}

void TcpBase::handleAction(const std::string&, const std::vector<std::string>&) {
	logDebug("TcpBase::handleAction not implemented");
}

void TcpBase::syncComm() {
	std::string message;
	while (this->getNextMessage(&message)) {
		size_t separator = message.find(' ');
		if (separator == std::string::npos) {
			this->handleAction(message, std::vector<std::string>());
		} else {
			std::string action = message.substr(0, separator);
			std::string parametersStr = message.substr(separator + 1);
			std::vector<std::string> parameters;
			split(parameters, parametersStr, is_any_of(" "));
			this->handleAction(action, parameters);
		}
	}
}

void TcpBase::sendAction(const std::string& action, const std::vector<std::string>& params) {
	this->sendMessage(action + " " + join(params, " "));
}

void TcpBase::sendAction(const std::string& action) {
	this->sendMessage(action);
}

}
