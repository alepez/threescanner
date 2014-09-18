/*
 * Quad.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Quad.h"

#include <vector>

namespace threescanner {

static const std::vector<float> VERTEX_DATA = { -1, 1, 0, 1, 1, 0, -1, -1, 0, -1, -1, 0, 1, 1, 0, 1, -1, 0 };

Quad::Quad() :
				vertexArrayID_(0),
				vertexBufferID_(0) {
	glGenVertexArrays(1, &vertexArrayID_);
	glBindVertexArray(vertexArrayID_);

	glGenBuffers(1, &vertexBufferID_);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VERTEX_DATA.size(), VERTEX_DATA.data(), GL_STATIC_DRAW);
}

Quad::~Quad() {
	glDeleteBuffers(1, &vertexBufferID_);
	glDeleteVertexArrays(1, &vertexArrayID_);
}

void Quad::render() {
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID_);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glDrawArrays(GL_TRIANGLES, 0, VERTEX_DATA.size() / 3);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

} /* namespace threescanner */
