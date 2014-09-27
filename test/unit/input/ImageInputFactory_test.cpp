#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <input/ImageInputFactory.h>

using namespace ::threescanner;
using namespace ::testing;

TEST(ImageInputFactory_, CanInstantiateFsType) {
	Config cfg = Config("fsInput.json");
	ASSERT_NO_THROW(ImageInputFactory::create("fs", cfg));
}

TEST(ImageInputFactory_, CanInstantiateWithTypeSpecifiedInConfig) {
	Config cfg = Config("fsInput.json");
	cfg.set<std::string>("type", "fs");
	ASSERT_NO_THROW(ImageInputFactory::create(cfg));
}
