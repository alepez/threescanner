/*
 * ProjectorFactory.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_PROJECTORFACTORY_H_
#define threescanner_PROJECTORFACTORY_H_

#include "../prerequisites.h"

#include <string>

namespace threescanner {

class ProjectorFactory {
public:
	static ProjectorPtr create(const std::string& type, const Config& cfg);
	static ProjectorPtr create(const Config& cfg);
};

} /* namespace threescanner */

#endif /* threescanner_PROJECTORFACTORY_H_ */
