#include "Shader.h"

Shader::Shader() {}
Shader::Shader(const char* vertexShaderPath, const char* fragShaderPath) {
	generate(vertexShaderPath, fragShaderPath); 
}

void Shader::generate(const char* vertexShaderPath, const char* fragmentShaderPath) {
	int success;  //success of the compilation of shader code 
	char infolog[512]; //to store error message if there is one 

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

// uniform functions
//glUniform4f(vertexColorLoc, 0.0f, 0.0f, blueValue, 1.0f);
void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set4Float(const std::string& name, float v1, float v2, float v3, float v4) {
	glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}


void Shader::setMat4(const std::string& name, glm::mat4 val) {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val)); 
}


