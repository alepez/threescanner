/*
 * ImageInput.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_IMAGEINPUT_H_
#define threescanner_IMAGEINPUT_H_

#include "../prerequisites.h"
#include "Image.h"

namespace threescanner {

class ImageInput {
public:
	static ImageInputPtr create(const std::string& type, const Config& cfg);
	static ImageInputPtr create(const Config& cfg);
	ImageInput(const Config& cfg);
	virtual ~ImageInput();
	virtual ImagePtr grabImage() = 0;
};

} /* namespace threescanner */

#endif /* threescanner_IMAGEINPUT_H_ */
