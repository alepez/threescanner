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
};

} /* namespace threescanner */

#endif /* threescanner_THREEPHASEPROJECTOR_H_ */
