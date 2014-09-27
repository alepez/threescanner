/*
 * Engine.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_ENGINE_H_
#define threescanner_ENGINE_H_

#include "../prerequisites.h"
#include "PointCloud.h"

#include <opencv2/core/core.hpp>
#include <string>
#include <memory>
#include <future>

namespace threescanner {

class Engine {
public:
	virtual ~Engine();
	std::future<void> scan();
	virtual void setParameter(const std::string& key, const std::string& value) = 0;
	virtual void setImage(const std::string& id, const cv::Mat& image) = 0;
	PointCloud::ConstPtr getCloud() const;
	void setInput(ImageInputPtr input);
	void connectProjector(ProjectorPtr);
protected:
	Engine(const Config&);
	virtual void scanSync() = 0;
	ImageInputPtr input_;
	PointCloud::Ptr cloud_;
	ProjectorPtr projector_;
private:
	std::future<void> projectorFuture_;
};

} /* namespace threescanner */

#endif /* threescanner_ENGINE_H_ */
