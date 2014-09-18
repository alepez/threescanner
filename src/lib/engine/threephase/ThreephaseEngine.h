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

struct RawPointCloud {
	static const size_t MAX_SIZE = (1024 * 768);
	size_t size_;
	float data_[3 * MAX_SIZE];
};

class ThreephaseEngine: public Engine {
public:
	ThreephaseEngine(const Config& cfg);
	virtual ~ThreephaseEngine();
private:
	virtual void setParameter(const std::string& key, const std::string& value);
	void setOutput(RawPointCloud* rawCloud);
	void setImage(const std::string& orientation, const size_t& phase, const cv::Mat& image);
	void process(const std::string& orientation = "last");
	void setOption(const std::string& key, const float& value);
	void loadImagesFromFs(const std::string& filePath);
	void setCrop(const std::string& key, const float& value);
	void setTrans(const std::string& key, const float& value);
	void setRotation(const std::string& key, const float& value);
	void setScale(const std::string& key, const float& value);
	void updateAffine();
	/**/
	void setup();
	void blurMask();
	void wrap(float noiseThreshold);
	void unwrap();
	void unwrap(float basePhase, int x, int y);
	void computeDepth(float zscale, float zskew);
	uchar mix(int r, int c);
	void saveToCloud();

	const bool debugEnabled_;
	const int wrapMethod_;

	std::map<std::string, float> options_;
	std::map<std::string, float> crop_;

	glm::vec3 translate_;
	glm::vec3 rotate_;
	glm::vec3 scale_;
	glm::mat4 affine_;

	typedef std::vector<cv::Point> Points;
	Points toProcess_;

	cv::Mat hImages_[3];
	cv::Mat vImages_[3];

	cv::Mat* phases_;
	cv::Mat process_;
	cv::Mat unwrapped_;
	cv::Mat mask_;

	cv::Mat depth_;

	RawPointCloud* rawCloud_;
};

} /* namespace threescanner */

#endif /* threescanner_THREEPHASEENGINE_H_ */
