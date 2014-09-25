#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <common/Logger.h>
#include <projector/RealProjector.h>
#include <future>

using namespace ::threescanner;
using namespace ::testing;

class RealProjectorInstance: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("projector") };
	ProjectorPtr projector { Projector::create("threephase", cfg) };
	RealProjector* realProjector { dynamic_cast<RealProjector*>(projector.get()) };
};

TEST_F(RealProjectorInstance, ShouldAcceptValidAction) {
	ASSERT_NO_THROW(realProjector->handleAction("quit"));
}

TEST_F(RealProjectorInstance, ShouldThrowExceptionWithWrongAction) {
	ASSERT_THROW(realProjector->handleAction("wrongAction!"), std::invalid_argument);
}
