#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <common/Logger.h>
#include <projector/threephase/ThreephaseProjector.h>
#include <future>

using namespace ::threescanner;
using namespace ::testing;

class ThreephaseProjectorInstance: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("projector") };
	ThreephaseProjector projector { (cfg) };
};

TEST_F(ThreephaseProjectorInstance, CanShowPhases) {
	projector.setParameter("orientation", "h");
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	for (int phase = 1; phase <= 3; ++phase) {
		logDebug("Scan phase %i", phase);
		projector.setParameter("phase", fmt::sprintf("%i", phase));
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
