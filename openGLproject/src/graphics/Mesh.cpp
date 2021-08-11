#include "Mesh.h"

std::vector<struct Vertex> Vertex::genList(float* vertices, int numVertices) {
	std::vector<Vertex> ret(numVertices); //vector of vertices

	int stride = sizeof(Vertex) / sizeof(float); //number of floats per vertex

	//positions of vertices
	for (int i = 0; i < numVertices; i++) {
		ret[i].pos = glm::vec3(
			vertices[i * stride + 0],
			vertices[i * stride + 1],
			vertices[i * stride + 2]
		);
		//texture coordinates of vertices
		ret[i].texCoord = glm::vec2(
			vertices[i * stride + 3],
			vertices[i * stride + 4]
		);
	}

	return ret; 
}

//default constructor
Mesh::Mesh() {}

//constructor with parameters
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) 
	: vertices(vertices), indices(indices), textures(textures) {
	setup();
}

void Mesh::render(Shader shader) {
	//activate textures
	for (unsigned int i = 0; i < textures.size(); i++) {
		shader.setInt(textures[i].name, textures[i].id); //get texture name and id from texture objects and set them in the shader program  
		glActiveTexture(GL_TEXTURE0 + i); //set the ith slot to active 
		textures[i].bind();
	}

	//draw shapes
	glBindVertexArray(VAO); 
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); 

	//reset active textures
	glActiveTexture(GL_TEXTURE0); 
}

//delete VAO, VBO, EBO 
void Mesh::cleanup() {
	glDeleteVertexArrays(1, &VAO); 
	glDeleteBuffers(1, &VBO); 
	glDeleteBuffers(1, &EBO); 
}


//set up VAO, VBO, EBO
void Mesh::setup() {
	//VAO
	glGenVertexArrays(1, &VAO); //Vertex array object (VAO), contains pointers to the vertex buffers
	glGenBuffers(1, &VBO); //Vertex buffer object (VBO), buffer containining vertex data 
	glGenBuffers(1, &EBO); 

	glBindVertexArray(VAO); // passes in an integer and openGL will know which array object is the active one

	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); //data for the VBO

	//EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); //data for the EBO

	//set vertex attribute pointers
	//set pointers for vertex.position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0); 
	

	//set pointers for vertex.texCoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(1);
	

	glBindVertexArray(0); 
}

