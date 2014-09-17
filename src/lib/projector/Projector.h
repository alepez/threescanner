/*
 * Projector.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_PROJECTOR_H_
#define threescanner_PROJECTOR_H_

#include "../prerequisites.h"

namespace threescanner {

class Projector {
protected:
	Projector(const Config&);
	virtual ~Projector();
};

} /* namespace threescanner */

#endif /* threescanner_PROJECTOR_H_ */
