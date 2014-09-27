/*
 * EngineFactory.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_ENGINEFACTORY_H_
#define threescanner_ENGINEFACTORY_H_

#include "../prerequisites.h"
#include <string>

namespace threescanner {

class EngineFactory {
public:
	static EnginePtr create(const std::string& type, const Config& cfg);
	static EnginePtr create(const Config& cfg);
};

} /* namespace threescanner */

#endif /* threescanner_ENGINEFACTORY_H_ */
