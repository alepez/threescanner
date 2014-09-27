/*
 * Projector.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_PROJECTOR_H_
#define threescanner_PROJECTOR_H_

#include "../prerequisites.h"

#include <string>
#include <future>

namespace threescanner {

class Projector {
public:
	virtual void setParameter(const std::string& key, const std::string& value)  = 0;
	virtual std::future<void> start() = 0;
	virtual void stop() = 0;
	virtual bool isReady() const = 0;
	virtual void waitUntilReady() const;
protected:
	Projector(const std::string& type, const Config&);
	virtual ~Projector();
	const std::string engineType_;
};

} /* namespace threescanner */

#endif /* threescanner_PROJECTOR_H_ */
