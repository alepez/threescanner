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
#include <future>

struct GLFWwindow;

namespace threescanner {

class Projector: public TcpServer {
public:
	static ProjectorPtr create(const std::string& type, const Config& cfg);
	static ProjectorPtr create(const Config& cfg);
	virtual void handleAction(const std::string& action, const std::vector<std::string>& params = std::vector<std::string>());
	virtual void setParameter(const std::string& key, const std::string& value);
	std::future<void> start();
	void stop();
protected:
	Projector(const std::string& type, const Config&);
	virtual ~Projector();
	GLuint getProgramID();
private:
	void run();
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
