#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <common/Config.h>
#include <input/cv/CVImageInput.h>

using namespace ::threescanner;
using namespace ::testing;

/**
 * need to share resource between tests, because real camera
 * cannot be relased so quickly.
 * Luckily, this is the only test case where a real camera is
 * needed.
 */
class ACVImageInput: public Test {
public:
	static CVImageInput* imageInput;
	static void SetUpTestCase() {
		Config cfg { Config("cvInput.json") };
		ASSERT_NO_THROW(imageInput = new CVImageInput(cfg));
	}
	static void TearDownTestCase() {
		delete imageInput;
		imageInput = nullptr;
	}
};

CVImageInput* ACVImageInput::imageInput = nullptr;

TEST_F(ACVImageInput, CanGrabImage) {
	ASSERT_THAT(imageInput, NotNull());
	ImagePtr image = imageInput->grabImage();
	ASSERT_THAT(image->cols, Not(0));
	ASSERT_THAT(image->rows, Not(0));
}

TEST_F(ACVImageInput, CanGrabImagesContinuously) {
	ASSERT_THAT(imageInput, NotNull());
	for (int i = 0; i < 25; ++i) {
		ImagePtr image = imageInput->grabImage();
		ASSERT_THAT(image->cols, Not(0));
		ASSERT_THAT(image->rows, Not(0));
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}
}
