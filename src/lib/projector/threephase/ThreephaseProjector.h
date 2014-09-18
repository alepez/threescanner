/*
 * ThreephaseProjector.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_THREEPHASEPROJECTOR_H_
#define threescanner_THREEPHASEPROJECTOR_H_

#include "../Projector.h"

namespace threescanner {

class ThreephaseProjector: public Projector {
public:
	ThreephaseProjector(const Config& cfg);
	virtual ~ThreephaseProjector();
private:
	virtual void setParameters(const std::string& key, const std::string& value);
};

} /* namespace threescanner */

#endif /* threescanner_THREEPHASEPROJECTOR_H_ */
