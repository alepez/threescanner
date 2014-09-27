#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <scanner/threephase/ThreephaseEngine.h>
#include <input/ImageInput.h>
#include <projector/ProjectorFactory.h>
#include <projector/Projector.h>

#include <format.h>
#include <opencv2/highgui/highgui.hpp>

using namespace ::threescanner;
using namespace ::testing;

class ThreephaseEngine_: public testing::Test {
public:
	std::string confFilepath { "threescanner.json" };
	Config cfg { Config(confFilepath).getChild("scanner").getChild("engine") };
	ThreephaseEngine engine { (cfg) };
};

TEST_F(ThreephaseEngine_, CanInstantiate) {
	ASSERT_NO_THROW(ThreephaseEngine instance(cfg));
}

TEST_F(ThreephaseEngine_, IsEngine) {
	ASSERT_THAT(dynamic_cast<Engine*>(&engine), NotNull());
}

TEST_F(ThreephaseEngine_, CanSetHorizontalImages) {
	for (size_t phase = 0; phase < 1; ++phase) {
		auto filepath = fmt::sprintf("phase%u.png", phase + 1);
		cv::Mat image = cv::imread(filepath);
		auto imageId = fmt::sprintf("h:%u", phase + 1);
		ASSERT_NO_THROW(engine.setImage(imageId, image));
	}
}

TEST_F(ThreephaseEngine_, CanSetParameters) {
	Config tuningCfg = cfg.getChild("tuning");
	ASSERT_NO_THROW(engine.setParameter("tuning:mscale", tuningCfg.get<std::string>("mscale")));
	ASSERT_NO_THROW(engine.setParameter("tuning:zscale", tuningCfg.get<std::string>("zscale")));
	ASSERT_NO_THROW(engine.setParameter("tuning:zskew", tuningCfg.get<std::string>("zskew")));
	ASSERT_NO_THROW(engine.setParameter("tuning:cloudScale", tuningCfg.get<std::string>("cloudScale")));
	ASSERT_NO_THROW(engine.setParameter("tuning:znoise", tuningCfg.get<std::string>("znoise")));
	ASSERT_NO_THROW(engine.setParameter("tuning:zblur", tuningCfg.get<std::string>("zblur")));
}

TEST_F(ThreephaseEngine_, CannotSetInvalidParameters) {
	Config tuningCfg = cfg.getChild("tuning");
	ASSERT_THROW(engine.setParameter("mscale", tuningCfg.get<std::string>("mscale")), std::invalid_argument);
	ASSERT_THROW(engine.setParameter("tuning", tuningCfg.get<std::string>("zscale")), std::invalid_argument);
	ASSERT_THROW(engine.setParameter("tuni**skew", tuningCfg.get<std::string>("zskew")), std::invalid_argument);
	ASSERT_THROW(engine.setParameter("tuningd:cloudScale", tuningCfg.get<std::string>("cloudScale")), std::invalid_argument);
}

TEST_F(ThreephaseEngine_, CanAttachInput) {
	ASSERT_NO_THROW(engine.setInput(ImageInput::create(Config("fsInput.json"))));
}

TEST_F(ThreephaseEngine_, CanScan) {
	ProjectorPtr projector = ProjectorFactory::create(Config("projector.json"));
	engine.connectProjector(projector);
	engine.setInput(ImageInput::create(Config("fsInput.json")));
	ASSERT_NO_THROW(engine.scanSync());
}

TEST_F(ThreephaseEngine_, ShouldNotScanWithoutAValidInput) {
	ASSERT_THROW(engine.scanSync(), std::runtime_error);
}

class ThreephaseEngineAfterScanning: public testing::Test {
public:
	std::string confFilepath { "threescanner.json" };
	Config cfg { Config(confFilepath).getChild("scanner").getChild("engine") };
	ThreephaseEngine engine { (cfg) };
	ImageInputPtr input = ImageInput::create(Config("fsInput.json"));
	ProjectorPtr projector = ProjectorFactory::create(Config("projector.json"));
	void SetUp() {
		engine.connectProjector(projector);
		engine.setInput(input);
		engine.scanSync();
	}
};

TEST_F(ThreephaseEngineAfterScanning, CanGetCloud) {
	static const size_t MIN_POINTS_IN_CLOUD = 1000;
	PointCloud::ConstPtr cloud = engine.getCloud();
	ASSERT_THAT(cloud->size(), Gt(MIN_POINTS_IN_CLOUD));
}

