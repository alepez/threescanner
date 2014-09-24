#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <input/ImageInput.h>

using namespace ::threescanner;
using namespace ::testing;

TEST(ImageInput_, CanInstantiateFsType) {
	Config cfg = Config("fsInput.json");
	ASSERT_NO_THROW(ImageInput::create("fs", cfg));
}

TEST(ImageInput_, CanInstantiateWithTypeSpecifiedInConfig) {
	Config cfg = Config("fsInput.json");
	cfg.set<std::string>("type", "fs");
	ASSERT_NO_THROW(ImageInput::create(cfg));
}
