/*
 * Engine.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_ENGINE_H_
#define threescanner_ENGINE_H_

#include "../prerequisites.h"

namespace threescanner {

class Engine {
public:
	static Engine* create(const Config& cfg);
	static void destroy(Engine*);
protected:
	Engine(const Config&);
	virtual ~Engine();
};

} /* namespace threescanner */

#endif /* threescanner_ENGINE_H_ */
