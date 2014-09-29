/*
 * ThreephaseProjector.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_THREEPHASEPROJECTOR_H_
#define threescanner_THREEPHASEPROJECTOR_H_

#include "../RealProjector.h"

#include <future>

namespace threescanner {

class ThreephaseProjector: public RealProjector {
public:
	ThreephaseProjector(const Config& cfg);
	virtual ~ThreephaseProjector();
	virtual void setParameter(const std::string& key, const std::string& value) override;
	virtual bool isReady() const override;
private:
	void delayChangeAccepted();
	bool changeAccepted_;
	std::future<void> changeAcceptedFuture_;
};

} /* namespace threescanner */

#endif /* threescanner_THREEPHASEPROJECTOR_H_ */
