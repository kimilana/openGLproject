#include "Model.h"

Model::Model() {}

//the init method will be specified by the specific models which inherit from the model class 
void Model::init() {}

//render the model by rendering each mesh in the model
void Model::render(Shader shader) {
	for (Mesh mesh : meshes) { //for each mesh in the vector of meshes
		mesh.render(shader); 
	}
}

//cleanup each mesh in the model
void Model::cleanup() {
	for (Mesh mesh : meshes) {
		mesh.cleanup(); 
	}
}

