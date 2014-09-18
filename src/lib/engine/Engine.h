/*
 * Engine.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_ENGINE_H_
#define threescanner_ENGINE_H_

#include "../prerequisites.h"

#include <string>
#include <opencv2/core/core.hpp>

namespace threescanner {

class Engine {
public:
	void run();
	virtual ~Engine();
protected:
	Engine(const Config&);
	virtual void setParameter(const std::string& key, const std::string& value);
	virtual void setImage(const std::string& id, const cv::Mat& image) = 0;
};

} /* namespace threescanner */

#endif /* threescanner_ENGINE_H_ */
