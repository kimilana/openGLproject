#include "Texture.h"
#include <iostream>

int Texture::currentId = 0; 

Texture::Texture() {}

Texture::Texture(const char* path, const char* name, bool defaultParams)
	: path(path), name(name), id(currentId++) { //automatically increments the id once we have used it
	generate(); //generates texture

	if (defaultParams) {
		setFilters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR); 
		setWrap(GL_REPEAT); 
	}
}

//returns the slot of memory on the GPU for the texture
void Texture::generate() {
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex); //tells openGL that the given texture is the active texture. On the GPU, the first texture unit points to the given texture  
}

//loads the texture image
void Texture::load(bool flip) {
	stbi_set_flip_vertically_on_load(true); //by default images load upside down so we must flip the image

	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0); 
	GLenum colorMode = GL_RGB; //default is 3 color channels which is RGB 
	switch (nChannels) {
	case 1: //1 color channel
		colorMode = GL_RED; 
		break;
	case 4: //4 color channels 
		colorMode = GL_RGBA; 
		break;
	}

	if (data) {
		glBindTexture(GL_TEXTURE_2D, id); 
		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data); //specify a two-dimensional texture image. (target texture, level-of-detail number, number of color components, width, height, border (must be 0), format of pixel data, type of pixel data, data (specifies a pointer to the image data in memory)
		glGenerateMipmap(GL_TEXTURE_2D); //mipmaps store different resolutions of an image
	}
	else { //if the texture didn't load 
		std::cout << "Image not loaded at " << path << std::endl;
	}

	stbi_image_free(data); //free the loaded image from memory
}

//sets both filters at the same time
void Texture::setFilters(GLenum all) {
	setFilters(all, all); 
}

//sets filters individually
void Texture::setFilters(GLenum mag, GLenum min) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min); 
}

//set how the texture wraps around an object
void Texture::setWrap(GLenum all) {
	setWrap(all, all); 
}
void Texture::setWrap(GLenum s, GLenum t) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t); 
}

//binds texture
void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id); 
}

