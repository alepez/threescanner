/*
 * NetProjector.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_NETPROJECTOR_H_
#define threescanner_NETPROJECTOR_H_

#include "../prerequisites.h"
#include "../net/TcpClient.h"

namespace threescanner {

class NetProjector: public TcpClient {
public:
	NetProjector(const Config&);
	virtual ~NetProjector();
	void setParameter(const std::string& key, const std::string& value);
	void quit();
};

} /* namespace threescanner */

#endif /* threescanner_NETPROJECTOR_H_ */
