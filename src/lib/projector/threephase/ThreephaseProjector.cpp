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
				Projector("threephase", cfg) {

}

ThreephaseProjector::~ThreephaseProjector() {

}

void ThreephaseProjector::setParameter(const std::string& key, const std::string& value) {
	static const int VERTICAL = 1;
	static const int HORIZONTAL = 0;
	const GLuint programID = this->getProgramID();
	if (key == "orientation") {
		if (value[0] == 'V' || value[0] == 'v') {
			glUniform1i(glGetUniformLocation(programID, "orientation"), VERTICAL);
		} else if (value[0] == 'H' || value[0] == 'h') {
			glUniform1i(glGetUniformLocation(programID, "orientation"), HORIZONTAL);
		} else {
			logError("Invalid orientation: " + value);
			/* do not throw exception */
			return;
		}
		return;
	}
	if (key == "phase") {
		glUniform1f(glGetUniformLocation(programID, "phase"), boost::lexical_cast<float>(value));
		return;
	}
}

} /* namespace threescanner */
