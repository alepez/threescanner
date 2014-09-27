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
	ImageInput(const Config& cfg);
	virtual ~ImageInput();
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual ImagePtr grabImage() = 0;
};

} /* namespace threescanner */

#endif /* threescanner_IMAGEINPUT_H_ */
