/*
 * FsImageInput.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_FSIMAGEINPUT_H_
#define threescanner_FSIMAGEINPUT_H_

#include "../ImageInput.h"

namespace threescanner {

class FsImageInput: public ImageInput {
public:
	FsImageInput(const Config& cfg);
	virtual ~FsImageInput();
	virtual ImagePtr grabImage();
	virtual void start();
	virtual void stop();
private:
	std::vector<ImagePtr> images_;
	size_t index_;
};

} /* namespace threescanner */

#endif /* threescanner_FSIMAGEINPUT_H_ */
