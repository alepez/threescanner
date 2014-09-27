#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <scanner/EngineFactory.h>
#include <input/ImageInput.h>
#include <projector/NetProjector.h>

using namespace ::threescanner;
using namespace ::testing;

class EngineFactory_: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("scanner").getChild("engine") };
};

TEST_F(EngineFactory_, CanInstantiateThreephaseType) {
	ASSERT_NO_THROW(EngineFactory::create("threephase", cfg));
}

TEST_F(EngineFactory_, CanInstantiateWithTypeSpecifiedInConfig) {
	cfg.set<std::string>("type", "threephase");
	ASSERT_NO_THROW(EngineFactory::create(cfg));
}

TEST_F(EngineFactory_, ShouldNotInstantiateWrongType) {
	ASSERT_THROW(EngineFactory::create("wrong!!", cfg), std::invalid_argument);
}
