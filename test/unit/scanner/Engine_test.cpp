#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <scanner/Engine.h>
#include <input/ImageInput.h>

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
	engine->setInput(input);
	auto f = engine->scan();
	f.wait();
}
