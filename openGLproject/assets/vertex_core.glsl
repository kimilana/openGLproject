#version 330 core
layout (location = 0) in vec3 aPos; //attribute for position
layout (location = 1) in vec2 aTexCoord; //attribute for texture coordinates


//out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 

uniform mat4 mouseTransform; 


void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0); 
	TexCoord = aTexCoord; 

}