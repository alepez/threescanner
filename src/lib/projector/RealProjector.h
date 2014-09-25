/*
 * RealProjector.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_REALPROJECTOR_H_
#define threescanner_REALPROJECTOR_H_

#include "../prerequisites.h"
#include "../net/TcpServer.h"
#include "Projector.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <future>

struct GLFWwindow;

namespace threescanner {

class RealProjector: public Projector, public TcpServer {
public:
	RealProjector(const std::string& type, const Config&);
	virtual ~RealProjector();
	virtual void handleAction(const std::string& action, const std::vector<std::string>& params = std::vector<std::string>());
	virtual std::future<void> start();
	virtual void stop();
	void run();
protected:
	GLuint getProgramID();
private:
	void setupWindow(const Config& cfg);
	void render();
	const Config* cfg_;
	GLFWwindow* window_;
	Quad* quad_;
	GLuint programID_;
	bool closeWindow_;
	int windowWidth_;
	int windowHeight_;
};

} /* namespace threescanner */

#endif /* threescanner_REALPROJECTOR_H_ */
