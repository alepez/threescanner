/*
 * Projector.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_PROJECTOR_H_
#define threescanner_PROJECTOR_H_

#include "../prerequisites.h"
#include "../net/TcpServer.h"

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace threescanner {

class Projector: public TcpServer {
public:
	static ProjectorPtr create(const std::string& type, const Config& cfg);
	static ProjectorPtr create(const Config& cfg);
	void run();
	virtual void handleAction(const std::string& action, const std::vector<std::string>& params = std::vector<std::string>());
protected:
	Projector(const std::string& type, const Config&);
	virtual ~Projector();
	virtual void setParameters(const std::string& key, const std::string& value);
	GLuint getProgramID();
private:
	void setupWindow(const Config& cfg);
	void render();
	const std::string engineType_;
	GLFWwindow* window_;
	Quad* quad_;
	GLuint programID_;
	bool closeWindow_;
	int windowWidth_;
	int windowHeight_;
};

} /* namespace threescanner */

#endif /* threescanner_PROJECTOR_H_ */
