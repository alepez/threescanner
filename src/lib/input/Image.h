/*
 * Image.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_IMAGE_H_
#define threescanner_IMAGE_H_

#include <opencv2/core/core.hpp>
#include <memory>

namespace threescanner {

/**
 * Images are handled by cv::Mat (OpenCV)
 * \see http://docs.opencv.org/modules/core/doc/basic_structures.html
 */
typedef cv::Mat Image;

/**
 * Shared pointer to Image
 */
typedef std::shared_ptr<Image> ImagePtr;

}

#endif /* threescanner_IMAGE_H_ */
