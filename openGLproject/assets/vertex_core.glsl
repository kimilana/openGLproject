#version 330 core
layout (location = 0) in vec3 aPos; //attribute for position
layout (location = 1) in vec3 aColor; //second attribute for color
layout (location = 2) in vec2 aTexCoord; //attribute for texture coordinates


out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform; //set in code 


void main() {
	gl_Position = transform * vec4(aPos, 1.0); 
	//gl_Position = vec4(aPos, 1.0); 
	ourColor = aColor; 
	TexCoord = aTexCoord; 

}