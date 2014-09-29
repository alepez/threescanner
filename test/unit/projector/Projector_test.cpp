#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <common/Logger.h>
#include <projector/Projector.h>
#include <projector/ProjectorFactory.h>
#include <future>

using namespace ::threescanner;
using namespace ::testing;

class AProjector: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("projector") };
	ProjectorPtr projector { ProjectorFactory::create("threephase", cfg) };
};

TEST_F(AProjector, CanStartAndStopAfterSomeTime) {
	static const int TIMEOUT_MS = 500;
	auto f = projector->start();
	std::async(std::launch::async, [&]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(TIMEOUT_MS));
		projector->stop();
	});
	f.wait();
}

class ARunningProjector: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("projector") };
	ProjectorPtr projector { ProjectorFactory::create("threephase", cfg) };
	void SetUp() {
		prjFuture = projector->start();
	}
	void TearDown() {
		projector->stop();
	}
	std::future<void> prjFuture;
};

TEST_F(ARunningProjector, CanBecomesReadyAfterSomeTime) {
	static const int TIMEOUT_MS = 1000;
	bool ready = false;
	auto fut = std::async(std::launch::async, [&]() {
		ASSERT_NO_THROW(projector->waitUntilReady());
		ready = true;
	});
	fut.wait_for(std::chrono::milliseconds(TIMEOUT_MS));
	ASSERT_TRUE(ready);
}
