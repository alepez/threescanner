/*
 * Projector.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_PROJECTOR_H_
#define threescanner_PROJECTOR_H_

#include "../prerequisites.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace threescanner {

class Projector {
public:
	void run();
protected:
	Projector(const Config&);
	virtual ~Projector();
private:
	void setupWindow(const Config& cfg);
	void render();
	GLFWwindow* window_;
	GLuint mvpID_;
	GLuint programID_;
	bool closeWindow_;
	int windowWidth_;
	int windowHeight_;
};

} /* namespace threescanner */

#endif /* threescanner_PROJECTOR_H_ */
