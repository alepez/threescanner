#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <input/fs/FsImageInput.h>

using namespace ::threescanner;
using namespace ::testing;

class FsImageInput_: public testing::Test {
public:
	Config cfg { Config("fsInput.json") };
	FsImageInput imageInput { cfg };
};

TEST_F(FsImageInput_, CanInstantiateFsType) {
	ASSERT_NO_THROW(FsImageInput instance(cfg));
}

TEST_F(FsImageInput_, CanGrabImage) {
	ImagePtr image = imageInput.grabImage();
	ASSERT_THAT(image->cols, Not(0));
	ASSERT_THAT(image->rows, Not(0));
}

TEST_F(FsImageInput_, CanGrabImagesContinuously) {
	for (int i = 0; i < 100; ++i) {
		ImagePtr image = imageInput.grabImage();
		ASSERT_THAT(image->cols, Not(0));
		ASSERT_THAT(image->rows, Not(0));
	}
}
