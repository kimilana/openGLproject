#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	int success;
	char infolog[512];

	GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER); 

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id); 

	//catch errors
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infolog);
		std::cout << "Linking error: " << std::endl << infolog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader); 
}

//tells OpenGL to use a shader
void Shader::activate() {
	glUseProgram(id);
}


//loads the shader source code 
std::string Shader::loadShaderSrc(const char* filename) {
	std::ifstream file;
	std::stringstream buf; //buffer to read the data into

	std::string ret = ""; //placeholder for the return variable 

	file.open(filename); //open the shader code file

	if (file.is_open()) { //if the file is open, read the file into the buffer
		buf << file.rdbuf(); //pipe readbuffer into stringbuffer 
		ret = buf.str();
	}
	else {
		std::cout << "Could not open " << filename << std::endl;
	}

	file.close();

	return ret;
}

GLuint Shader::compileShader(const char* filepath, GLenum type) {
	int success;
	char infoLog[512];

	GLuint ret = glCreateShader(type); 
	std::string shaderSrc = loadShaderSrc(filepath);
	const GLchar* shader = shaderSrc.c_str(); 
	glShaderSource(ret, 1, &shader, NULL); 
	glCompileShader(ret); 

	//catch error
	glGetShaderiv(ret, GL_COMPILE_STATUS, &success); //tells openGL to get the shader value GL_COMPILE_STATUS to tell us if it was successful 
	if (!success) {
		glGetShaderInfoLog(ret, 512, NULL, infoLog);
		std::cout << "Error with fragment shader compilation. : " << std::endl << infoLog << std::endl;
	}

	return ret;
}

void Shader::setMat4(const std::string& name, glm::mat4 val) {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val)); 
}


