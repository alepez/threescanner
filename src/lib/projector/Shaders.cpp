/*
 * Shaders.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: alessandro.pezzato@gmail.com
 */

#include "Shaders.h"
//#include <utils/Format.h>
//#include <utils/log.h>
#include <fstream>
#include <vector>
#include <algorithm>

namespace threescanner {

std::map<std::string, GLuint> Shaders::shaders_c;

GLuint Shaders::load(const std::string& name) {
	GLuint programID = glCreateProgram();
	Shaders::loadShader(programID, name, GL_VERTEX_SHADER);
	Shaders::loadShader(programID, name, GL_GEOMETRY_SHADER);
	Shaders::loadShader(programID, name, GL_FRAGMENT_SHADER);
	glLinkProgram(programID);

	/* Check the program */
	GLint result = GL_FALSE;
	int infoLen = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLen);
	char* errorMessage = new char[infoLen];
	glGetProgramInfoLog(programID, infoLen, NULL, errorMessage);
	if (errorMessage[0]) {
// TODO		LOG_DEBUG(Format("%s", errorMessage));
	}
	delete errorMessage;

	shaders_c[name] = programID;
	return programID;
}

void Shaders::loadShader(GLuint programID, const std::string& name, const GLenum type) {
	std::string typeName;

	switch (type) {
	case GL_VERTEX_SHADER:
		typeName = "vertex";
		break;
	case GL_FRAGMENT_SHADER:
		typeName = "fragment";
		break;
	case GL_GEOMETRY_SHADER:
		typeName = "geometry";
		break;
	}

	// FIXME: path must be valid when installed
	std::string filePath = "../../assets/engines/" + name + "/shaders/" + typeName + ".glsl";

	GLuint shaderID = glCreateShader(type);

	std::string shaderCode;
	std::ifstream fs(filePath.c_str(), std::ios::in);
	if (fs.is_open()) {
		std::string line;
		while (getline(fs, line)) {
			shaderCode += "\n" + line;
		}
		fs.close();
	} else {
		/* ignora, file non leggibile */
		return;
	}

	// Compile Vertex Shader
// TODO	LOG_DEBUG("Compiling shader: " + filePath);
	const char* source = shaderCode.c_str();
	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);

	// Check Vertex Shader
	GLint result = GL_FALSE;
	int infoLen = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLen);
	char* errorMessage = new char[infoLen];
	glGetShaderInfoLog(shaderID, infoLen, NULL, errorMessage);
	if (errorMessage[0]) {
// TODO		LOG_DEBUG(errorMessage);
	}
	delete errorMessage;

	glAttachShader(programID, shaderID);

	glDeleteShader(shaderID);
}

GLuint Shaders::get(const std::string& name) {
	if (!shaders_c.count(name)) {
		Shaders::load(name);
	}
	return shaders_c[name];
}

void Shaders::destroy(const std::string& name) {
	glDeleteProgram(Shaders::get(name));
}

}
