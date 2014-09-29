/*
 * ThreephaseProjector.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "ThreephaseProjector.h"
#include "../../common/Logger.h"

#include <boost/lexical_cast.hpp>

namespace threescanner {

ThreephaseProjector::ThreephaseProjector(const Config& cfg) :
				RealProjector("threephase", cfg),
				changeAccepted_(true) {

}

ThreephaseProjector::~ThreephaseProjector() {

}

void ThreephaseProjector::setParameter(const std::string& key, const std::string& value) {
	changeAccepted_ = false;
	static const int VERTICAL = 1;
	static const int HORIZONTAL = 0;
	const GLuint programID = this->getProgramID();
	if (key == "orientation") {
		if (value[0] == 'V' || value[0] == 'v') {
			logDebug("Change orientation to VERTICAL");
			/* TODO: glUniform1i must be called in graphic thread */
//			glUniform1i(glGetUniformLocation(programID, "orientation"), VERTICAL);
		} else if (value[0] == 'H' || value[0] == 'h') {
			logDebug("Change orientation to HORIZONTAL");
			/* TODO: glUniform1i must be called in graphic thread */
//			glUniform1i(glGetUniformLocation(programID, "orientation"), HORIZONTAL);
		} else {
			logError("Invalid orientation: " + value);
			/* do not throw exception */
		}
		return this->delayChangeAccepted();
	}
	if (key == "phase") {
		logDebug("Change phase to " + value);
		/* TODO: glUniform1i must be called in graphic thread */
//		glUniform1f(glGetUniformLocation(programID, "phase"), boost::lexical_cast<float>(value));
		return this->delayChangeAccepted();
	}
}

void ThreephaseProjector::delayChangeAccepted() {
	static const int DELAY_MS = 500;
	changeAcceptedFuture_ = std::async(std::launch::async, [&]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
		changeAccepted_ = true;
	});
}

bool ThreephaseProjector::isReady() const {
	return this->RealProjector::isReady() && changeAccepted_;
}

} /* namespace threescanner */
