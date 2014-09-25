/*
 * ThreephaseProjector.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_THREEPHASEPROJECTOR_H_
#define threescanner_THREEPHASEPROJECTOR_H_

#include "../RealProjector.h"

namespace threescanner {

class ThreephaseProjector: public RealProjector {
public:
	ThreephaseProjector(const Config& cfg);
	virtual ~ThreephaseProjector();
private:
	virtual void setParameter(const std::string& key, const std::string& value);
};

} /* namespace threescanner */

#endif /* threescanner_THREEPHASEPROJECTOR_H_ */
