#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

struct Vertex {
	glm::vec3 pos; //3D position vector
	glm::vec2 texCoord; //texture coordinates 

	static std::vector<struct Vertex> genList(float* vertices, int numVertices); 
};
typedef struct Vertex Vertex; 

class Mesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	std::vector<Texture> textures; 

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures); 

	void render(Shader shader); 

	void cleanup();

private: 
	unsigned int VBO, EBO; 

	void setup();
};

#endif