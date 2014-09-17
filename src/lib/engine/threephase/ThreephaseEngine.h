/*
 * ThreephaseEngine.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_THREEPHASEENGINE_H_
#define threescanner_THREEPHASEENGINE_H_

#include "../Engine.h"

namespace threescanner {

class ThreephaseEngine: public Engine {
public:
	ThreephaseEngine(const Config& cfg);
	virtual ~ThreephaseEngine();
};

} /* namespace threescanner */

#endif /* threescanner_THREEPHASEENGINE_H_ */
