#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <input/cv/CVImageInput.h>

using namespace ::threescanner;
using namespace ::testing;

/*
 * Cannot use fixture here, camera device cannot
 * be open/closed so quickly.
 */
Config cfg { Config("cvInput.json") };
CVImageInput imageInput { cfg };

TEST(CVImageInput_, CanGrabImage) {
	ImagePtr image = imageInput.grabImage();
	ASSERT_THAT(image->cols, Not(0));
	ASSERT_THAT(image->rows, Not(0));
}

TEST(CVImageInput_, CanGrabImagesContinuously) {
	for (int i = 0; i < 25; ++i) {
		ImagePtr image = imageInput.grabImage();
		ASSERT_THAT(image->cols, Not(0));
		ASSERT_THAT(image->rows, Not(0));
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}
}
