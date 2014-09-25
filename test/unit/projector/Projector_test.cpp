#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <common/Logger.h>
#include <projector/Projector.h>
#include <future>

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


class ProjectorInstance: public testing::Test {
public:
	Config cfg { Config("threescanner.json").getChild("projector") };
	ProjectorPtr projector { Projector::create("threephase", cfg) };
};

TEST_F(ProjectorInstance, CanStartAndStopAfterSomeTime) {
	static const int TIMEOUT_MS = 500;
	auto f = projector->start();
	std::async(std::launch::async, [&](){
		std::this_thread::sleep_for(std::chrono::milliseconds(TIMEOUT_MS));
		projector->stop();
	});
	f.wait();
}