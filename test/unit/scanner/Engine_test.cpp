#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <scanner/EngineFactory.h>
#include <scanner/Engine.h>
#include <input/ImageInput.h>
#include <input/ImageInputFactory.h>
#include <projector/NetProjector.h>

using namespace ::threescanner;
using namespace ::testing;

class AnEngine: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("scanner").getChild("engine") };
	EnginePtr engine = EngineFactory::create("threephase", cfg);
	ImageInputPtr input = ImageInputFactory::create(Config("fsInput.json"));
};

TEST_F(AnEngine, CanSetInput) {
	ASSERT_NO_THROW(engine->setInput(input));
}

TEST_F(AnEngine, CannotSetInputIfAlreadyPresent) {
	engine->setInput(input);
	ASSERT_THROW(engine->setInput(input), std::invalid_argument);
}

TEST_F(AnEngine, CanScanAsyncronously) {
	static const int TIMEOUT_MS = 1000;
	engine->setInput(input);
	auto fut = engine->scan();
	std::future_status status = fut.wait_for(std::chrono::milliseconds(TIMEOUT_MS));
	ASSERT_THAT(status, Eq(std::future_status::ready));
}

TEST_F(AnEngine, CanConnectProjector) {
	ProjectorPtr projector = std::make_shared<NetProjector>(cfg.getChild("projector"));
	engine->connectProjector(projector);
}
