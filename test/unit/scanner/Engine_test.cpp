#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <scanner/Engine.h>
#include <input/ImageInput.h>
#include <projector/NetProjector.h>

using namespace ::threescanner;
using namespace ::testing;

class Engine_: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("scanner").getChild("engine") };
};

TEST_F(Engine_, CanInstantiateThreephaseType) {
	ASSERT_NO_THROW(Engine::create("threephase", cfg));
}

TEST_F(Engine_, CanInstantiateWithTypeSpecifiedInConfig) {
	cfg.set<std::string>("type", "threephase");
	ASSERT_NO_THROW(Engine::create(cfg));
}

TEST_F(Engine_, ShouldNotInstantiateWrongType) {
	ASSERT_THROW(Engine::create("wrong!!", cfg), std::invalid_argument);
}

class EngineInstance: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("scanner").getChild("engine") };
	EnginePtr engine = Engine::create("threephase", cfg);
	ImageInputPtr input = ImageInput::create(Config("fsInput.json"));
};

TEST_F(EngineInstance, CanSetInput) {
	ASSERT_NO_THROW(engine->setInput(input));
}

TEST_F(EngineInstance, CannotSetInputIfAlreadyPresent) {
	engine->setInput(input);
	ASSERT_THROW(engine->setInput(input), std::invalid_argument);
}

TEST_F(EngineInstance, CanScanAsyncronously) {
	static const int TIMEOUT_MS = 1000;
	engine->setInput(input);
	auto fut = engine->scan();
	std::future_status status = fut.wait_for(std::chrono::milliseconds(TIMEOUT_MS));
	ASSERT_THAT(status, Eq(std::future_status::ready));
}

//TEST_F(EngineInstance, CanConnectProjector) {
//	NetProjector projector(cfg.getChild("projector"));
//	engine->connectProjector(std::make_shared<Projector>(projector));
//}
