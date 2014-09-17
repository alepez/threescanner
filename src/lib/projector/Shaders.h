#ifndef threescanner_SHADERS_H_
#define threescanner_SHADERS_H_

#include <GL/glew.h>
#include <map>
#include <string>

namespace threescanner {

class Shaders {
public:
	static GLuint get(const std::string& name);
	static void destroy(const std::string& name);
	static void destroy(const GLuint& id);
private:
	static GLuint load(const std::string& name);
	static void loadShader(GLuint programID, const std::string& name, const GLenum type);
	static std::map<std::string, GLuint> shaders_c;
};

}

#endif /* threescanner_SHADERS_H_ */
