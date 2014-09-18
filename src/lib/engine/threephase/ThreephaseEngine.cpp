/*
 * ThreephaseEngine.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "ThreephaseEngine.h"

//#include <calibration/ThreePhase.h>
//#include <calibration/SharedPointCloud.h>
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

namespace threescanner {

/* Documentazione dell'algoritmo:
 *  - http://public.vrac.iastate.edu/~song/publications/papers/2010-ole-review.pdf
 */

/*
 * PHASE_GAMMA_LUT e` inizializzato prima di chiamare main()
 */
static const int WRAP_V_SIZE = 511; // i1 - i3, -255 to 255, |511|
static const int WRAP_U_SIZE = 1021; // 2 * i2 - i1 - i3, -510 to 510, |1021|
static const int WRAP_V_OFFSET = 255; // min value is -255
static const int WRAP_U_OFFSET = 510; // min value is -510
static unsigned char PHASE_GAMMA_LUT[WRAP_V_SIZE][WRAP_U_SIZE][2];
static void initPhaseGammaLut() __attribute__ ((constructor)); /* FIXME: solo gcc */
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

static bool isVertical(const std::string& orientation) {
	return orientation[0] == 'v' || orientation[0] == 'V';
}

static bool isHorizontal(const std::string& orientation) {
	return orientation[0] == 'h' || orientation[0] == 'H';
}

struct RawPoint {
	float x, y, z;
};

static inline bool isInside(glm::vec4 p, const float* box) {
	if (p.x < box[0]) {
		return false;
	}
	if (p.x > box[1]) {
		return false;
	}
	if (p.y < box[2]) {
		return false;
	}
	if (p.y > box[3]) {
		return false;
	}
	if (p.z < box[4]) {
		return false;
	}
	if (p.z > box[5]) {
		return false;
	}
	return true;
}

static void traceMatrix(const std::string& name, const glm::mat4& mat) {
	const float* a = (const float*) glm::value_ptr(mat);
	logDebug("%16s | %8.3f %8.3f %8.3f %8.3f", name.c_str(), a[0], a[4], a[8], a[12]);
	logDebug("                 | %8.3f %8.3f %8.3f %8.3f", a[1], a[5], a[9], a[13]);
	logDebug("                 | %8.3f %8.3f %8.3f %8.3f", a[2], a[6], a[10], a[14]);
	logDebug("                 | %8.3f %8.3f %8.3f %8.3f", a[3], a[7], a[11], a[15]);
}

ThreephaseEngine::ThreephaseEngine(const Config& cfg) :
				Engine(cfg),
				debugEnabled_(cfg.get<bool>("debug")),
				wrapMethod_(cfg.get<int>("wrapMethod")),
				options_(),
				toProcess_(),
				hImages_ { },
				vImages_ { },
				phases_(hImages_),
				process_(),
				unwrapped_(),
				mask_(),
				depth_(),
				rawCloud_(nullptr) {

	/* tuning */
	Config tuning = cfg.getChild("tuning");

	options_["mscale"] = tuning.get<float>("mscale", 200.0);
	options_["zscale"] = tuning.get<float>("zscale", 151.0);
	options_["zskew"] = tuning.get<float>("zskew", 180.0);
	options_["cloudScale"] = tuning.get<float>("cloudScale", 1.0);
	options_["znoise"] = tuning.get<float>("znoise", 0.720);
	options_["zblur"] = tuning.get<float>("zblur", 16);

	/* translate */
	Config translate = cfg.getChild("translate");
	translate_.x = translate.get<float>("x", 0.0);
	translate_.y = translate.get<float>("y", 0.0);
	translate_.z = translate.get<float>("z", 0.0);

	/* rotate */
	Config rotate = cfg.getChild("rotate");
	rotate_.x = rotate.get<float>("x", 0.0);
	rotate_.y = rotate.get<float>("y", 0.0);
	rotate_.z = rotate.get<float>("z", 0.0);

	/* scale */
	Config scale = cfg.getChild("scale");
	scale_.x = scale.get<float>("x", 1.0);
	scale_.y = scale.get<float>("y", 1.0);
	scale_.z = scale.get<float>("z", 1.0);

	this->updateAffine();

	/* crop */
	Config crop = cfg.getChild("crop");
	crop_["xmin"] = crop.get<float>("xmin", -1.0);
	crop_["xmax"] = crop.get<float>("xmax", 1.0);
	crop_["ymin"] = crop.get<float>("ymin", -1.0);
	crop_["ymax"] = crop.get<float>("ymax", 1.0);
	crop_["zmin"] = crop.get<float>("zmin", -1.0);
	crop_["zmax"] = crop.get<float>("zmax", 1.0);
}

ThreephaseEngine::~ThreephaseEngine() {

}

void ThreephaseEngine::updateAffine() {
	glm::mat4 translation = glm::translate(translate_);

	static const glm::vec3 AXIS_X = glm::vec3(1.0, 0.0, 0.0);
	static const glm::vec3 AXIS_Y = glm::vec3(0.0, 1.0, 0.0);
	static const glm::vec3 AXIS_Z = glm::vec3(0.0, 0.0, 1.0);
	glm::mat4 rotx = glm::rotate(rotate_.x, AXIS_X);
	glm::mat4 roty = glm::rotate(rotate_.y, AXIS_Y);
	glm::mat4 rotz = glm::rotate(rotate_.z, AXIS_Z);
//	traceMatrix("rotx", rotx);
//	traceMatrix("roty", roty);
//	traceMatrix("rotz", rotz);

	glm::mat4 scale = glm::scale(scale_);

	//affine_ = glm::mat4(1.0) * translation * rotx * roty * rotz * scale;
	affine_ = glm::mat4(1.0);
	affine_ = affine_ * translation;
	affine_ = affine_ * rotx;
	affine_ = affine_ * roty;
	affine_ = affine_ * rotz;
	affine_ = scale * affine_; /* scale va applicata dopo tutto per mantenere il centro */

//	traceMatrix("transformation", affine_);
}

void ThreephaseEngine::loadImagesFromFs(const std::string& filePathPattern) {
	for (size_t phase = 0; phase < 3; ++phase) {
		auto filepath = fmt::sprintf(filePathPattern.c_str(), phase + 1);
		hImages_[phase] = cv::imread(filepath, CV_LOAD_IMAGE_GRAYSCALE);
		if (hImages_[phase].rows) {
			logInfo("Loaded test image for phase %i from %s %i", phase + 1, filepath.c_str(), hImages_[phase].flags);
		}
	}
}

void ThreephaseEngine::setImage(const std::string& orientation, const size_t& phase, const cv::Mat& image) {
	logDebug("ThreePhase save image %u", phase);
	if (phase > 2) {
		throw std::runtime_error(fmt::sprintf("ThreePhase invalid phase: %u", phase));
	}

	cv::Mat* phases = isVertical(orientation) ? vImages_ : hImages_;
	cv::Mat& dst = phases[phase];
	cv::cvtColor(image, dst, CV_RGB2GRAY);
}

void ThreephaseEngine::process(const std::string& orientation) {
	if (isVertical(orientation)) {
		phases_ = vImages_;
	} else if (isHorizontal(orientation)) {
		phases_ = hImages_;
	} else if (orientation == "last") {
		if (phases_ == nullptr) {
			throw std::runtime_error("Last orientation is undefined");
		}
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
	this->saveToCloud();

	if (debugEnabled_) {
		cv::imshow(fmt::sprintf("phase%u", 0 + 1), phases_[0]);
		cv::imshow(fmt::sprintf("phase%u", 1 + 1), phases_[1]);
		cv::imshow(fmt::sprintf("phase%u", 2 + 1), phases_[2]);

		cv::Mat dm;
		depth_.convertTo(dm, CV_8UC1);
		cv::Mat dmj;
		cv::applyColorMap(dm, dmj, cv::COLORMAP_JET);
		cv::imshow("depth", dmj);

		cv::Mat ms;
		mask_.convertTo(ms, CV_8U, 200);
		cv::imshow("mask", ms);

		cv::Mat uv;
		unwrapped_.convertTo(uv, CV_8U, 80, 127);
		cv::imshow("unwrapped", uv);

		cv::Mat pm;
		process_.convertTo(pm, CV_8UC1, 200);
		cv::imshow("process", pm);
	}
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

uchar ThreephaseEngine::mix(int r, int c) {
	return uchar(int(phases_[0].at<uchar>(r, c) + phases_[1].at<uchar>(r, c) + phases_[2].at<uchar>(r, c)) / 3);
}

void ThreephaseEngine::setup() {
	int w = phases_[0].cols;
	int h = phases_[0].rows;
	logDebug("ThreePhase setup %ux%u", w, h);
	unwrapped_ = cv::Mat::zeros(h, w, CV_32F);
	depth_ = cv::Mat::zeros(h, w, CV_32F);
	mask_ = cv::Mat::zeros(h, w, CV_8U);
	process_ = cv::Mat::zeros(h, w, CV_8U);
}

void ThreephaseEngine::setOutput(RawPointCloud* rawCloud) {
	rawCloud_ = rawCloud;
}

void ThreephaseEngine::saveToCloud() {
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
	const float cloudScale = options_["cloudScale"];
	const float scale = zscale - mscale;
	float skew = zskew - mscale;
	if (skew == 0.0) {
		skew = 1.0;
	}
	int width = depth_.cols;
	int height = depth_.rows;

	RawPoint* rowCloudData = reinterpret_cast<RawPoint*>(rawCloud_->data_);
	RawPoint* rcd = rowCloudData;

	RawPoint barycenter = { 0.0, 0.0, 0.0 };

	float pointsNum = 0.0;

	/* ottimizzazione */
	float crop[] = { crop_["xmin"], crop_["xmax"], crop_["ymin"], crop_["ymax"], crop_["zmin"], crop_["zmax"] };

	for (int y = 0; y < height; ++y) {
		float planephase = 0.5 + float(y - (height / 2)) / skew;
		for (int x = 0; x < width; ++x) {
			if (!mask_.at<uchar>(y, x)) {
				float z = (unwrapped_.at<float>(y, x) - planephase) * scale;
				/* WARNING: y z  sono invertiti */
				/* vec4 serve solo per la trasformazione affine, in realta` basterebbe vec3
				 * w e` 1.0 e non 0.0 perche` altrimenti non funziona la traslazione
				 */
				glm::vec4 p(x * cloudScale, -y * cloudScale, -z * cloudScale, 1.0);
				p = affine_ * p;
				if (isInside(p, crop)) {
					rcd->x = p.x;
					rcd->y = p.y;
					rcd->z = p.z;
					barycenter.x += p.x;
					barycenter.y += p.y;
					barycenter.z += p.z;
					pointsNum += 1.0;
					++rcd;
				}
			}
		}
	}

	rawCloud_->size_ = pointsNum;

	barycenter.x /= pointsNum;
	barycenter.y /= pointsNum;
	barycenter.z /= pointsNum;

	logDebug("Points: %.0f", pointsNum);
	logDebug("Barycenter: %.3f, %.3f, %.3f", barycenter.x, barycenter.y, barycenter.z);
}

void ThreephaseEngine::setOption(const std::string& key, const float& value) {
	if (!options_.count(key)) {
		logWarning("ThreePhase option does not exist: " + key);
	}
	options_[key] = value;
}

void ThreephaseEngine::setCrop(const std::string& key, const float& value) {
	if (!crop_.count(key)) {
		logWarning("ThreePhase crop key does not exist: " + key);
	}
	crop_[key] = value;
	this->saveToCloud();
}

void ThreephaseEngine::setTrans(const std::string& key, const float& value) {
	if (key.size() != 1) {
		/* ignora */
		return;
	}
	switch (key[0]) {
	case 'x':
		translate_.x = value;
		break;
	case 'y':
		translate_.y = value;
		break;
	case 'z':
		translate_.z = value;
		break;
	}
	this->updateAffine();
}

void ThreephaseEngine::setRotation(const std::string& key, const float& value) {
	if (key.size() != 1) {
		/* ignora */
		return;
	}
	switch (key[0]) {
	case 'x':
		rotate_.x = value;
		break;
	case 'y':
		rotate_.y = value;
		break;
	case 'z':
		rotate_.z = value;
		break;
	}
	this->updateAffine();
}

void ThreephaseEngine::setScale(const std::string& key, const float& value) {
	if (key.size() != 1) {
		/* ignora */
		return;
	}
	switch (key[0]) {
	case 'x':
		scale_.x = value;
		break;
	case 'y':
		scale_.y = value;
		break;
	case 'z':
		scale_.z = value;
		break;
	case '*':
		scale_.x = value;
		scale_.y = value;
		scale_.z = value;
		break;
	}
	this->updateAffine();
}

void ThreephaseEngine::setParameter(const std::string& key, const std::string& value) {
	/* must be reimplemented by derived class */
}

} /* namespace threescanner */
