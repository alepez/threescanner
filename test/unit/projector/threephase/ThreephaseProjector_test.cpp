#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <common/Logger.h>
#include <projector/threephase/ThreephaseProjector.h>
#include <future>

using namespace ::threescanner;
using namespace ::testing;

class AThreephaseProjector: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("projector") };
	ThreephaseProjector projector { (cfg) };
};

class ARunningThreephaseProjector: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("projector") };
	ThreephaseProjector projector { (cfg) };
	void SetUp() {
		logDebug("projector.start()");
		prjFuture = projector.start();
		projector.waitUntilReady();
	}
	void TearDown() {
		projector.stop();
	}
	std::future<void> prjFuture;
};

TEST_F(ARunningThreephaseProjector, BecomesReadyAfterSomeTimeWhenOrientationChanges) {
	static const int TIMEOUT_MS = 1000;
	bool ready = false;
	auto fut = std::async(std::launch::async, [&]() {
		std::string orientation = "h";
		projector.setParameter("orientation", orientation);
		ASSERT_NO_THROW(projector.waitUntilReady());
		ready = true;
	});
	fut.wait_for(std::chrono::milliseconds(TIMEOUT_MS));
	ASSERT_TRUE(ready);
}

TEST_F(ARunningThreephaseProjector, BecomesReadyAfterSomeTimeWhenPhaseChanges) {
	static const int TIMEOUT_MS = 1000;
	for (int phase = 1; phase <= 3; ++phase) {
		bool ready = false;
		auto fut = std::async(std::launch::async, [&]() {
			std::string orientation = "h";
			projector.setParameter("phase", fmt::sprintf("%i", phase));
			ASSERT_NO_THROW(projector.waitUntilReady());
			ready = true;
		});
		fut.wait_for(std::chrono::milliseconds(TIMEOUT_MS));
		ASSERT_TRUE(ready);
	}
}
