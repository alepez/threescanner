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
	std::async(std::launch::async, [&](){
		std::this_thread::sleep_for(std::chrono::milliseconds(TIMEOUT_MS));
		projector->stop();
	});
	f.wait();
}
