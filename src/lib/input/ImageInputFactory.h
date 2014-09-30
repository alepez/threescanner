/*
 * ImageInputFactory.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_IMAGEINPUTFACTORY_H_
#define threescanner_IMAGEINPUTFACTORY_H_

#include "../prerequisites.h"

namespace threescanner {

/**
 * Factory to create instances of ImageInput
 *
 * Available types:
 *
 *  - fs (filesystem) FsImageInput
 *  - cv (opencv camera) CVImageInput
 */
class ImageInputFactory {
public:
	/**
	 * Create an instance of ImageInput, given the type and a Config object
	 * \param type the implementation type
	 * \param cfg a Config object
	 */
	static ImageInputPtr create(const std::string& type, const Config& cfg);
	/**
	 * \param cfg a Config object. The implementation type must be defined
	 * inside the Config object in the path `type`.
	 */
	static ImageInputPtr create(const Config& cfg);
};

} /* namespace threescanner */

#endif /* threescanner_IMAGEINPUTFACTORY_H_ */
