/*
 * NetProjector.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_NETPROJECTOR_H_
#define threescanner_NETPROJECTOR_H_

#include "../prerequisites.h"
#include "../net/TcpClient.h"
#include "Projector.h"

namespace threescanner {

class NetProjector: public Projector, public TcpClient {
public:
	NetProjector(const Config&);
	virtual ~NetProjector();
	void setParameter(const std::string& key, const std::string& value) override;
	virtual std::future<void> start() override;
	virtual void stop() override;
	virtual bool isReady() const override;
	void quit();
};

} /* namespace threescanner */

#endif /* threescanner_NETPROJECTOR_H_ */
