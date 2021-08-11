#ifndef CUBE_HPP
#define CUBE_HPP

#include "../Model.h"
#include <iostream>

class Cube : public Model {
public:
	//position and size of cube
	glm::vec3 pos; 
	glm::vec3 size; 

	Cube(glm::vec3 pos, glm::vec3 size) //the cube object takes in two inputs, position and size
		: pos(pos), size(size) {}

	void init() {
    //vertices for a cube. 2 triangles per side * 6 sides * 3 vertices per triangle.
	int numVertices = 36; 
    
    //Each vertex is 3 postion coordinate components and 2 texture coordinate components
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
   };

      //indices
     std::vector<unsigned int> indices(numVertices);
     for (unsigned int i = 0; i < numVertices; i++) { 
         indices[i] = i; 
     }


     //textures
     Texture tex0("assets/marylandflag.jpg", "texture0"); 
     tex0.load();
     Texture tex1("assets/crab.jpg", "texture1"); 
     tex1.load();

     meshes.push_back(Mesh(Vertex::genList(vertices, numVertices), indices, { tex0, tex1 })); //pushback vector method adds the Mesh of cube data to the end of the meshes vector

	}

    //this method renders a cube
    void render(Shader shader) {

        //matrix transformations for the cube
        glm::mat4 model = glm::mat4(1.0f); 
        model = glm::translate(model, pos); //this transformation matrix puts the object in the position given by pos input 
        model = glm::scale(model, size); //this transforamtion matrix scales the object to the size given by size input
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f)); //this transformation rotates the cube
       
        

        //shader code
        shader.setMat4("model", model); 

        Model::render(shader); 

    }


};

#endif