#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <common/Logger.h>
#include <projector/ProjectorFactory.h>
#include <future>

using namespace ::threescanner;
using namespace ::testing;

class ProjectorFactory_: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("projector") };
};

TEST_F(ProjectorFactory_, CanInstantiateThreephaseType) {
	ASSERT_NO_THROW(ProjectorFactory::create("threephase", cfg));
}

TEST_F(ProjectorFactory_, CanInstantiateWithTypeSpecifiedInConfig) {
	cfg.set<std::string>("type", "threephase");
	ASSERT_NO_THROW(ProjectorFactory::create(cfg));
}

TEST_F(ProjectorFactory_, ShouldNotInstantiateWrongType) {
	ASSERT_THROW(ProjectorFactory::create("wrong!!", cfg), std::invalid_argument);
}
