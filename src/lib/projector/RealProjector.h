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
#include <queue>

struct GLFWwindow;

namespace threescanner {

class RealProjector: public Projector, public TcpServer {
public:
	RealProjector(const std::string& type, const Config&);
	virtual ~RealProjector();
	virtual void handleAction(const std::string& action, const std::vector<std::string>& params = std::vector<std::string>()) override;
	virtual std::future<void> start() override;
	virtual void stop() override;
	void run();
	virtual bool isReady() const override;
protected:
	GLuint getProgramID();
	/**
	 * Push a function that will run in the OpenGL thread.
	 * Why? GL functions, like glUniform* must run in the same
	 * thread as OpenGL.
	 */
	void pushChange(std::function<void()>);
private:
	void setupWindow(const Config& cfg);
	void render();
	void applyChanges();
	const Config* cfg_;
	GLFWwindow* window_;
	Quad* quad_;
	GLuint programID_;
	bool closeWindow_;
	int windowWidth_;
	int windowHeight_;
	bool ready_;
	std::queue<std::function<void()>> changesQueue_;
};

} /* namespace threescanner */

#endif /* threescanner_REALPROJECTOR_H_ */
