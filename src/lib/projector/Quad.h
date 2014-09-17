/*
 * Quad.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_QUAD_H_
#define threescanner_QUAD_H_

#include <GL/glew.h>
#include <string>

namespace threescanner {

class Quad {
public:
	Quad();
	virtual ~Quad();
	void render();
private:
	GLuint vertexArrayID_;
	GLuint vertexBufferID_;
};

} /* namespace threescanner */

#endif /* threescanner_QUAD_H_ */
