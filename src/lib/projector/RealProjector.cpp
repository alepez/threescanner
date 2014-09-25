/*
 * RealProjector.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "RealProjector.h"
#include "../common/Config.h"
#include "../common/Logger.h"
#include "threephase/ThreephaseProjector.h"
#include "Quad.h"
#include "Shaders.h"

#include <GLFW/glfw3.h>
#include <boost/algorithm/string.hpp>

namespace threescanner {

static GLFWmonitor* selectMonitor(const std::string& name);

RealProjector::RealProjector(const std::string& type, const Config& cfg) :
				Projector(type, cfg),
				TcpServer(cfg.getChild("net")),
				cfg_(new Config(cfg)),
				window_(nullptr),
				quad_(nullptr),
				programID_(0),
				closeWindow_(false),
				windowWidth_(0),
				windowHeight_(0),
				ready_(false) {

}

RealProjector::~RealProjector() {
	this->stop();
	delete cfg_;
}

void RealProjector::handleAction(const std::string& action, const std::vector<std::string>& params) {
	if (action.empty()) {
		return;
	}
	logTrace(action + " => " + boost::join(params, " "));
	if (action == "quit") {
		closeWindow_ = true;
		return;
	}
	if ((action == "set") && params.size() == 2) {
		this->setParameter(params[0], params[1]);
		return;
	}
	throw std::invalid_argument("Projector: unknown action: " + action);
}

void RealProjector::setupWindow(const Config& cfg) {
	auto width = cfg.get<int>("width");
	auto height = cfg.get<int>("height");
	auto antialiasing = cfg.get<int>("antialiasing", 0);
	auto name = cfg.get<std::string>("name");
	glfwWindowHint(GLFW_SAMPLES, antialiasing);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window_ = glfwCreateWindow(width, height, name.c_str(), selectMonitor(cfg.get<std::string>("monitor", "none")), nullptr);
	if (!window_) {
		glfwTerminate();
		throw std::runtime_error("Failed to open GLFW window.");
	}
	glfwMakeContextCurrent(window_);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW");
	}
	glfwGetWindowSize(window_, &windowWidth_, &windowHeight_);
	glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	logDebug("Window size: %ix%i", windowWidth_, windowHeight_);
}

void RealProjector::run() {
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}
	this->setupWindow(cfg_->getChild("window"));
	quad_ = new Quad();
	programID_ = Shaders::get(engineType_);
	glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);
	logDebug("Projector is ready");
	ready_ = true;
	do {
		this->syncComm();
		this->render();
	} while ((glfwGetKey(window_, GLFW_KEY_ESCAPE) != GLFW_PRESS) && (!glfwWindowShouldClose(window_)) && !closeWindow_);

	Shaders::destroy(programID_);
	glfwTerminate();
	delete quad_;
}

void RealProjector::render() {
	static const glm::mat4 identity(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programID_);
	glUniform2f(glGetUniformLocation(programID_, "resolution"), windowWidth_, windowHeight_);
	quad_->render();
	glfwSwapBuffers(window_);
	glfwPollEvents();
}

GLuint RealProjector::getProgramID() {
	return programID_;
}

GLFWmonitor* selectMonitor(const std::string& name) {
	int count = 0;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	for (int i = 0; i != count; ++i) {
		GLFWmonitor* monitor = monitors[i];
		if (name == glfwGetMonitorName(monitor)) {
			return monitor;
		}
	}
	if (name != "none") {
		logError("Cannot find monitor " + name);
	}
	return nullptr;
}

std::future<void> RealProjector::start() {
	return std::async(std::launch::async, [this]() {
		this->run();
	});
}

void RealProjector::stop() {
	closeWindow_ = true;
}

bool RealProjector::isReady() const {
	return ready_;
}

} /* namespace threescanner */
