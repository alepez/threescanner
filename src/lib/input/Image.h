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

typedef cv::Mat Image;
typedef std::shared_ptr<Image> ImagePtr;

}

#endif /* threescanner_IMAGE_H_ */
