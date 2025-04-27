#include <iostream>
#include "compute_shader.h"
#include "shader/util.h"

ComputeShader::Guard::Guard(GLuint id):
	_id(id) {
	glUseProgram(_id);
}

ComputeShader::Guard::~Guard() {
	glUseProgram(0);
}

ComputeShader::Guard ComputeShader::use() const {
	return Guard(_id);
}

ComputeShader::ComputeShader(const std::string& src):
	_id(glCreateProgram()) {
   // Ensure compute shader support (OpenGL 4.3+)
   GLint major = 0, minor = 0;
   glGetIntegerv(GL_MAJOR_VERSION, &major);
   glGetIntegerv(GL_MINOR_VERSION, &minor);
   if (major < 4 || (major == 4 && minor < 3)) {
       std::cerr << "Error: OpenGL 4.3+ required for compute shaders. Current version: "
                 << major << "." << minor << std::endl;
       _good = false;
       return;
   }
   GLint shader = util::compileShader(src, GL_COMPUTE_SHADER);

	if ( shader != -1 ) {
		glAttachShader(_id, shader);
		glLinkProgram(_id);
		_good = true;
	}
}

ComputeShader::~ComputeShader() {
	glDeleteProgram(_id);
}

bool ComputeShader::isGood() const {
	return _good;
}

GLuint ComputeShader::setUniform(const std::string& name, float x, float y) const {
	GLuint id = util::getUniform(_id, name);
	glUniform2f(id, x, y);
	return id;
}

void ComputeShader::workOn(GLuint buffer) const {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer);
}

void ComputeShader::dispatch(std::size_t dimX) const {
	glDispatchCompute(dimX, 1, 1);
}
