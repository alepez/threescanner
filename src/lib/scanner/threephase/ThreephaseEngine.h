/*
 * ThreephaseEngine.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_THREEPHASEENGINE_H_
#define threescanner_THREEPHASEENGINE_H_

#include "../Engine.h"
#include "../../prerequisites.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <opencv2/core/core.hpp>

namespace threescanner {

class ThreephaseEngine: public Engine {
public:
	ThreephaseEngine(const Config& cfg);
	virtual ~ThreephaseEngine();
	virtual void setParameter(const std::string& key, const std::string& value);
	virtual void setImage(const std::string& id, const cv::Mat& image);
	virtual std::future<void> scan();
private:
	void setImage(const std::string& orientation, const size_t& phase, const cv::Mat& image);
	void process(const std::string& orientation = "last");
	void setOption(const std::string& key, const float& value);
	/**/
	void setup();
	void blurMask();
	void wrap(float noiseThreshold);
	void unwrap();
	void unwrap(float basePhase, int x, int y);
	void computeDepth(float zscale, float zskew);
	void createCloud();

	const int wrapMethod_;

	std::map<std::string, float> options_;

	typedef std::vector<cv::Point> Points;
	Points toProcess_;

	cv::Mat hImages_[3];
	cv::Mat vImages_[3];

	cv::Mat* phases_;
	cv::Mat process_;
	cv::Mat unwrapped_;
	cv::Mat mask_;

	cv::Mat depth_;
};

} /* namespace threescanner */

#endif /* threescanner_THREEPHASEENGINE_H_ */
