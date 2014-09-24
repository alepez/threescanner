#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <scanner/Engine.h>

#include <format.h>
#include <opencv2/highgui/highgui.hpp>

using namespace ::threescanner;
using namespace ::testing;

class Engine_: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("scanner").getChild("engine") };
};

TEST_F(Engine_, CanInstantiateThreephaseType) {
	ASSERT_NO_THROW(Engine::create("threephase", cfg, nullptr));
}

TEST_F(Engine_, CanInstantiateWithTypeSpecifiedInConfig) {
	cfg.set<std::string>("type", "threephase");
	ASSERT_NO_THROW(Engine::create(cfg, nullptr));
}

TEST_F(Engine_, ShouldNotInstantiateWrongType) {
	ASSERT_THROW(Engine::create("wrong!!", cfg, nullptr), std::invalid_argument);
}
