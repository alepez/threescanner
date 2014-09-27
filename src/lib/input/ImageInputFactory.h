/*
 * ImageInputFactory.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_IMAGEINPUTFACTORY_H_
#define threescanner_IMAGEINPUTFACTORY_H_

#include "../prerequisites.h"

namespace threescanner {

class ImageInputFactory {
public:
	static ImageInputPtr create(const std::string& type, const Config& cfg);
	static ImageInputPtr create(const Config& cfg);
};

} /* namespace threescanner */

#endif /* threescanner_IMAGEINPUTFACTORY_H_ */
