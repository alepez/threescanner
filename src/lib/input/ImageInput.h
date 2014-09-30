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

/**
 * Base class for image input.
 *
 * Derived classes must provide images from real data (camera, webcam...)
 * or fake data (images stored in filesystem)
 */
class ImageInput {
public:
	/**
	 * Constructor
	 *
	 * \param cfg the configuration for the instance to be created. For the
	 * configuration structure, refer to each implementation.
	 */
	ImageInput(const Config& cfg);
	/**
	 * Destructor
	 */
	virtual ~ImageInput();
	/**
	 * Start the input thread (if needed by the implementation)
	 */
	virtual void start() = 0;
	/**
	 * Stop the input thread
	 */
	virtual void stop() = 0;
	/**
	 * Grab an image
	 * \return a shared pointer to the Image
	 */
	virtual ImagePtr grabImage() = 0;
};

} /* namespace threescanner */

#endif /* threescanner_IMAGEINPUT_H_ */
