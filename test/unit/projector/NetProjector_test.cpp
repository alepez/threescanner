#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <common/Logger.h>
#include <projector/NetProjector.h>
#include <projector/ProjectorFactory.h>
#include <future>

using namespace ::threescanner;
using namespace ::testing;

class NetProjectorInstance: public testing::Test {
public:
	Config cfg { Config("projector.json") };
	ProjectorPtr realProjector { ProjectorFactory::create(cfg) };
	NetProjector projector { cfg };
};

TEST_F(NetProjectorInstance, CanSendQuitAndRealProjectorActuallyQuit) {
	static const int TIMEOUT_MS = 500;
	auto fut = realProjector->start();
	ASSERT_NO_THROW(projector.quit());
	std::future_status status = fut.wait_for(std::chrono::milliseconds(TIMEOUT_MS));
	ASSERT_THAT(status, Eq(std::future_status::ready));
}

TEST_F(NetProjectorInstance, CanSetParameter) {
	ASSERT_NO_THROW(projector.setParameter("x", "y"));
}
