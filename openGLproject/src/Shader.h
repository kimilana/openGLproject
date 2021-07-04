#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int id; 

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath); 
	void activate(); 

	//utility functions
	std::string loadShaderSrc(const char* filepath); //this function loads the shader source code
	GLuint compileShader(const char* filepath, GLenum type); //this function compiles the shader

	// uniform functions
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4);
	void setMat4(const std::string& name, glm::mat4 val); 

};

#endif