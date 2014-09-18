/*
 * Engine.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_ENGINE_H_
#define threescanner_ENGINE_H_

#include "../prerequisites.h"

#include <string>

namespace threescanner {

class Engine {
protected:
	Engine(const Config&);
	virtual ~Engine();
	virtual void setParameter(const std::string& key, const std::string& value);
};

} /* namespace threescanner */

#endif /* threescanner_ENGINE_H_ */
