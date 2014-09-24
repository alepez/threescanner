#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <scanner/Scanner.h>
#include <scanner/Engine.h>

#include <format.h>
#include <opencv2/highgui/highgui.hpp>

using namespace ::threescanner;
using namespace ::testing;

class Scanner_: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("scanner") };
};

TEST_F(Scanner_, CanInstantiateWithoutEngine) {
	ASSERT_NO_THROW(Scanner instance(cfg, nullptr));
}

TEST_F(Scanner_, CanInstantiateWithEngine) {
	Engine::Ptr engine = Engine::create("threephase", cfg.getChild("engine"), nullptr);
	ASSERT_NO_THROW(Scanner instance(cfg, engine.get()));
}
