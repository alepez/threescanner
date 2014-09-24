#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <scanner/Scanner.h>
#include <scanner/Engine.h>

using namespace ::threescanner;
using namespace ::testing;

class Scanner_: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("scanner") };
};

TEST_F(Scanner_, CanInstantiateWithoutEngine) {
	ASSERT_NO_THROW(Scanner instance(cfg));
}

TEST_F(Scanner_, CanInstantiateWithEngine) {
	EnginePtr engine = Engine::create("threephase", cfg.getChild("engine"));
	ASSERT_NO_THROW(Scanner instance(cfg, engine));
}

TEST_F(Scanner_, CanSetEngineAfterInitialization) {
	EnginePtr engine = Engine::create("threephase", cfg.getChild("engine"));
	Scanner scanner(cfg);
	scanner.setEngine(engine);
}
