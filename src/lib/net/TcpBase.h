/*
 * TcpBase.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_TCPBASE_H_
#define threescanner_TCPBASE_H_

#include "../prerequisites.h"

#include <boost/thread/mutex.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <queue>
#include <memory>

namespace threescanner {

class TcpBase {
public:
	TcpBase(const Config& cfg);
	virtual ~TcpBase();
	void sendAction(const std::string& action, const std::vector<std::string>& params);
	void sendAction(const std::string& action);
protected:
	virtual void handleAction(const std::string& action, const std::vector<std::string>& params);
	void syncComm();
	bool getNextMessage(std::string*);
	void sendMessage(const std::string&);
	void update(boost::asio::ip::tcp::socket& s);
	const std::string name_;
private:
	void sendQueuedMessages(boost::asio::ip::tcp::socket& s);
	void receiveMessages(boost::asio::ip::tcp::socket& s);
	typedef std::queue<std::string> Queue;
	Queue inQueue_;
	Queue outQueue_;
	boost::mutex mutex_;
};

}

#endif /* threescanner_TCPBASE_H_ */
