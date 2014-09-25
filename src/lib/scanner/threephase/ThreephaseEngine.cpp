/*
 * ThreephaseEngine.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "ThreephaseEngine.h"
#include "../../common/Logger.h"
#include "../../common/Config.h"

#include <format.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

/*
 * http://public.vrac.iastate.edu/~song/publications/papers/2010-ole-review.pdf
 */

namespace threescanner {

namespace /* anonymous */{

static const int WRAP_V_SIZE = 511; /* i1 - i3, -255 to 255, |511| */
static const int WRAP_U_SIZE = 1021; /* 2 * i2 - i1 - i3, -510 to 510, |1021| */
static const int WRAP_V_OFFSET = 255; /* min value is -255 */
static const int WRAP_U_OFFSET = 510; /* min value is -510 */
static unsigned char PHASE_GAMMA_LUT[WRAP_V_SIZE][WRAP_U_SIZE][2];

/*
 * initPhaseGammaLut() is automatically called once at startup
 */
void initPhaseGammaLut() __attribute__ ((constructor)); /* FIXME: only gcc is compatible */
void initPhaseGammaLut() {
	float sqrt3 = sqrtf(3.0);
	float ipi = float(128. / CV_PI);
	for (int vo = 0; vo < WRAP_V_SIZE; vo++) {
		for (int uo = 0; uo < WRAP_U_SIZE; uo++) {
			float v = float(vo - WRAP_V_OFFSET);
			float u = float(uo - WRAP_U_OFFSET);
			float phase = atan2f(sqrt3 * v, u) * ipi;
			float modulation = sqrtf(3 * v * v + u * u);
			PHASE_GAMMA_LUT[vo][uo][0] = 128 + (unsigned char) phase;
			PHASE_GAMMA_LUT[vo][uo][1] = modulation > 255 ? 255 : (unsigned char) modulation;
		}
	}
}

bool isVertical(const std::string& orientation) {
	return orientation[0] == 'v' || orientation[0] == 'V';
}

bool isHorizontal(const std::string& orientation) {
	return orientation[0] == 'h' || orientation[0] == 'H';
}

void traceMatrix(const std::string& name, const glm::mat4& mat) {
	const float* a = (const float*) glm::value_ptr(mat);
	logDebug("%16s | %8.3f %8.3f %8.3f %8.3f", name.c_str(), a[0], a[4], a[8], a[12]);
	logDebug("                 | %8.3f %8.3f %8.3f %8.3f", a[1], a[5], a[9], a[13]);
	logDebug("                 | %8.3f %8.3f %8.3f %8.3f", a[2], a[6], a[10], a[14]);
	logDebug("                 | %8.3f %8.3f %8.3f %8.3f", a[3], a[7], a[11], a[15]);
}

} /* namespace anonymous */

ThreephaseEngine::ThreephaseEngine(const Config& cfg) :
				Engine(cfg),
				wrapMethod_(cfg.get<int>("wrapMethod")),
				options_(),
				toProcess_(),
				hImages_ { },
				vImages_ { },
				phases_(hImages_),
				process_(),
				unwrapped_(),
				mask_(),
				depth_() {
	/* tuning */
	Config tuning = cfg.getChild("tuning");

	options_["mscale"] = tuning.get<float>("mscale", 200.0);
	options_["zscale"] = tuning.get<float>("zscale", 151.0);
	options_["zskew"] = tuning.get<float>("zskew", 180.0);
	options_["znoise"] = tuning.get<float>("znoise", 0.720);
	options_["zblur"] = tuning.get<float>("zblur", 16);
	options_["cloudScale"] = tuning.get<float>("cloudScale", 1.0);

}

ThreephaseEngine::~ThreephaseEngine() {

}

void ThreephaseEngine::scanSync() {
	/* TODO:
	 * should tell to projector to project the pattern
	 * should get images from ImageInput (Camera)
	 */
	std::string orientation = "h"; /* TODO: from Config */
	this->process(orientation);
}

void ThreephaseEngine::setImage(const std::string& orientation, const size_t& phase, const cv::Mat& image) {
	if (phase > 3) {
		throw std::runtime_error(fmt::sprintf("Threephase invalid phase: %u", phase));
	}
	cv::Mat* phases = isVertical(orientation) ? vImages_ : hImages_;
	cv::Mat& dst = phases[phase - 1];
	cv::cvtColor(image, dst, CV_RGB2GRAY);
	logDebug("Threephase set image %u", phase);
}

void ThreephaseEngine::process(const std::string& orientation) {
	if (isVertical(orientation)) {
		phases_ = vImages_;
	} else if (isHorizontal(orientation)) {
		phases_ = hImages_;
	} else if (orientation == "last" && phases_ == nullptr) {
		throw std::runtime_error("Last orientation is undefined");
	} else {
		throw std::runtime_error("Invalid orientation");
	}
	if (!phases_[0].rows || !phases_[1].rows || !phases_[2].rows) {
		logWarning("Cannot process empty images");
		return;
	}
	const int znoise = options_["znoise"];
	const float zscale = options_["zscale"];
	const float mscale = options_["mscale"];
	const float zskew = options_["zskew"];
	this->setup();
	this->wrap(znoise);
	this->unwrap();
	this->blurMask();
	this->computeDepth(zscale - mscale, zskew - mscale);
	this->createCloud();
}

void ThreephaseEngine::blurMask() {
	const int zblur = options_["zblur"];
	if (zblur) {
		cv::blur(mask_, mask_, cv::Size(zblur, zblur));
	}
}

void ThreephaseEngine::unwrap() {
	int width = phases_[0].cols;
	int height = phases_[0].rows;
	int startX = width / 2;
	int startY = height / 2;

	toProcess_.clear();

	toProcess_.push_back(cv::Point(startX, startY));
	process_.at<uchar>(startY, startX) = false;

	while (!toProcess_.empty()) {
		cv::Point p = toProcess_.back();
		toProcess_.pop_back();

		int x = p.x;
		int y = p.y;
		float r = unwrapped_.at<float>(y, x);
		if (y > 0) {
			this->unwrap(r, x, y - 1);
		}
		if (y < height - 2) {
			this->unwrap(r, x, y + 1);
		}
		if (x > 0) {
			this->unwrap(r, x - 1, y);
		}
		if (x < width - 2) {
			this->unwrap(r, x + 1, y);
		}
	}
	toProcess_.clear();
}

void ThreephaseEngine::unwrap(float basePhase, int x, int y) {
	const int idx = x + y * process_.cols;
	float* unwrappedBuffer = reinterpret_cast<float*>(unwrapped_.data);
	const float bfquo = basePhase - round(basePhase);
	if (process_.data[idx]) {
		float& p0 = unwrappedBuffer[idx];
		float diff = p0 - bfquo;
		if (diff > 0.5f) {
			diff -= 1.0;
		} else if (diff < -0.5f) {
			diff += 1.0;
		}
		p0 = basePhase + diff;
		process_.data[idx] = 0;
		toProcess_.push_back(cv::Point(x, y));
	}
}

void ThreephaseEngine::wrap(float noiseThreshold) {
	int width = process_.cols;
	int height = process_.rows;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			const int i = x + y * width;
			const int lum0 = phases_[0].data[i];
			const int lum1 = phases_[1].data[i];
			const int lum2 = phases_[2].data[i];
			const int v = lum0 - lum2;
			const int u = 2 * lum1 - lum0 - lum2;
			const unsigned char* const cur = PHASE_GAMMA_LUT[v + WRAP_V_OFFSET][u + WRAP_U_OFFSET];
			mask_.data[i] = (cur[1] < noiseThreshold * 64);

			process_.data[i] = !mask_.data[i];
			static const float SQRT3 = sqrtf(3.0);
			static const float TWO_PI = 6.283185;

			float p2;
			switch (wrapMethod_) {
			case 0:
				p2 = atan2(SQRT3 * (lum0 - lum2), 2.0f * lum1 - lum0 - lum2) / TWO_PI;
				break;
			case 1:
				p2 = (cur[0] - 128.0f) / 255.0f;
				break;
			default:
				logWarning("calibration.json engine.wrapMethod can be 0 or 1");
				break;
			}
			float* unwrappedBuffer = reinterpret_cast<float*>(unwrapped_.data);
			unwrappedBuffer[i] = p2;
		}
	}
}

void ThreephaseEngine::computeDepth(float zscale, float zskew) {
	float* unwrappedBuffer = reinterpret_cast<float*>(unwrapped_.data);
	float* depthBuffer = reinterpret_cast<float*>(depth_.data);

	for (int i = 0; i < unwrapped_.rows; ++i) {
		float planephase = float(i - unwrapped_.rows / 2);
		if (zskew) {
			planephase /= zskew;
		}
		planephase += 0.5f;
		for (int j = 0; j < unwrapped_.cols; ++j) {
			int ii = i * unwrapped_.cols + j;
			depthBuffer[ii] = mask_.data[ii] ? 0.0f : ((unwrappedBuffer[ii] - planephase) * zscale);
		}
	}
}

void ThreephaseEngine::setup() {
	int w = phases_[0].cols;
	int h = phases_[0].rows;
	logDebug("Threephase setup %ux%u", w, h);
	unwrapped_ = cv::Mat::zeros(h, w, CV_32F);
	depth_ = cv::Mat::zeros(h, w, CV_32F);
	mask_ = cv::Mat::zeros(h, w, CV_8U);
	process_ = cv::Mat::zeros(h, w, CV_8U);
}

void ThreephaseEngine::createCloud() {
	logDebug("Save to cloud");
	/*
	 * TODO: controlla che il risultato sia corretto prima di salvare la point cloud.
	 * A volte (causa delay telecamera) le immagini catturate non sono valide.
	 * Un'opzione per controllare se e` valido e` vedere se il numero di punti
	 * rispetta una soglia minima aspettata.
	 */
	const float zscale = options_["zscale"];
	const float mscale = options_["mscale"];
	const float zskew = options_["zskew"];
	const float scale = zscale - mscale;
	float skew = zskew - mscale;
	if (skew == 0.0) {
		skew = 1.0;
	}
	int width = depth_.cols;
	int height = depth_.rows;

	struct RawPoint {
		float x, y, z;
	};

	PointCloud& cloud = *cloud_;
	/*
	 * TODO: need optimization. cloud_ memory can be reserved in
	 * initialization, then only size change. But how? std::vector
	 * do not permit to change size without deallocating.
	 */
	cloud.clear();

	for (int y = 0; y < height; ++y) {
		float planephase = 0.5 + float(y - (height / 2)) / skew;
		for (int x = 0; x < width; ++x) {
			if (!mask_.at<uchar>(y, x)) {
				float z = (unwrapped_.at<float>(y, x) - planephase) * scale;
				Point p;
				p.x = x;
				p.y = y;
				p.z = z;
				p.r = 0xff;
				p.g = 0xff;
				p.b = 0xff;
				cloud.push_back(p);
			}
		}
	}

	logDebug("Points: %u", cloud.size());
}

void ThreephaseEngine::setOption(const std::string& key, const float& value) {
	if (!options_.count(key)) {
		throw std::invalid_argument("Threephase option does not exist: " + key);
	}
	options_[key] = value;
}

void ThreephaseEngine::setParameter(const std::string& key, const std::string& value) {
	std::vector<std::string> k;
	boost::split(k, key, boost::is_any_of(":"));
	if (k[0] == "tuning" && k.size() == 2) {
		return this->setOption(k[1], boost::lexical_cast<float>(value));
	}
	throw std::invalid_argument("Threephase invalid parameter: " + key);
}

void ThreephaseEngine::setImage(const std::string& id, const cv::Mat& image) {
	std::vector<std::string> params;
	boost::split(params, id, boost::is_any_of(":"));
	auto orientation = params[0];
	auto phase = boost::lexical_cast<size_t>(params[1]);
	this->setImage(orientation, phase, image);
}

} /* namespace threescanner */
