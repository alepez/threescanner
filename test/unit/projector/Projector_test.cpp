#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <projector/Projector.h>

using namespace ::threescanner;
using namespace ::testing;

class Projector_: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("projector") };
};

TEST_F(Projector_, CanInstantiateThreephaseType) {
	ASSERT_NO_THROW(Projector::create("threephase", cfg));
}

TEST_F(Projector_, CanInstantiateWithTypeSpecifiedInConfig) {
	cfg.set<std::string>("type", "threephase");
	ASSERT_NO_THROW(Projector::create(cfg));
}

TEST_F(Projector_, ShouldNotInstantiateWrongType) {
	ASSERT_THROW(Projector::create("wrong!!", cfg), std::invalid_argument);
}

TEST_F(Projector_, ShouldAcceptValidAction) {
	ProjectorPtr projector = Projector::create("threephase", cfg);
	ASSERT_NO_THROW(projector->handleAction("quit"));
}

TEST_F(Projector_, ShouldThrowExceptionWithWrongAction) {
	ProjectorPtr projector = Projector::create("threephase", cfg);
	ASSERT_THROW(projector->handleAction("wrongAction!"), std::invalid_argument);
}
