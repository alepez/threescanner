#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <scanner/Scanner.h>
#include <scanner/EngineFactory.h>

using namespace ::threescanner;
using namespace ::testing;

class AScanner: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("scanner") };
};

TEST_F(AScanner, CanInstantiateWithoutEngine) {
	ASSERT_NO_THROW(Scanner instance(cfg));
}

TEST_F(AScanner, CanInstantiateWithEngine) {
	EnginePtr engine = EngineFactory::create("threephase", cfg.getChild("engine"));
	ASSERT_NO_THROW(Scanner instance(cfg, engine));
}

TEST_F(AScanner, CanSetEngineAfterInitialization) {
	EnginePtr engine = EngineFactory::create("threephase", cfg.getChild("engine"));
	Scanner scanner(cfg);
	scanner.setEngine(engine);
}

class AnInitializedScanner: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("scanner") };
	EnginePtr engine { EngineFactory::create("threephase", cfg.getChild("engine")) };
	Scanner scanner { (cfg) };
	void SetUp() {
		scanner.setEngine(engine);
	}
};

TEST_F(AnInitializedScanner, CanStartAndStopAfterSomeTime) {
	static const int DELAY_MS = 750;
	auto f = scanner.start();
	std::async(std::launch::async, [&]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
		scanner.stop();
	});
	f.wait();
}
