#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Mesh.h"

//specific models such as cube will inherit from the model class

class Model {
public:
	std::vector<Mesh> meshes;

	Model(); 

	void init(); //the values in the meshes will be set up in this method 

	void render(Shader shader); 

	void cleanup(); 
};

#endif