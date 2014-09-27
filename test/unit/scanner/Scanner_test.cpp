#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <scanner/Scanner.h>
#include <scanner/EngineFactory.h>

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
	EnginePtr engine = EngineFactory::create("threephase", cfg.getChild("engine"));
	ASSERT_NO_THROW(Scanner instance(cfg, engine));
}

TEST_F(Scanner_, CanSetEngineAfterInitialization) {
	EnginePtr engine = EngineFactory::create("threephase", cfg.getChild("engine"));
	Scanner scanner(cfg);
	scanner.setEngine(engine);
}

class ScannerInstance: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("scanner") };
	EnginePtr engine { EngineFactory::create("threephase", cfg.getChild("engine")) };
	Scanner scanner { (cfg) };
	void SetUp() {
		scanner.setEngine(engine);
	}
};

TEST_F(ScannerInstance, CanStartAndStopAfterSomeTime) {
	auto f = scanner.start();
	std::async(std::launch::async, [&]() {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		scanner.stop();
	});
	f.wait();
}
