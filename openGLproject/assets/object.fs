#version 330 core
out vec4 FragColor; //output a 4D vector called FragColor. Because it's coming from the fragment shader OpenGL knows it will be the color of the fragment

in vec3 ourColor;
in vec2 TexCoord; 

uniform sampler2D texture1; //tells openGL we are sending in a texture ID which maps to a texture
uniform sampler2D texture2; 

uniform float mixVal; //this variable is set in the main method 


void main() {
	// FragColor = vec4(1.0f, 0.2f, 0.6f, 1.0f);  //set color. this is in normalized values from 0 to 1 rather than from 0 to 255 in RGB. f stands for float. To get the equivalent color in RGB, multiply by 255. The last value is the alpha value in RGBA. 
	//FragColor = vec4(ourColor, 1.0); //take in a color from the vertex shader
	//FragColor =  texture(texture1, TexCoord); //use texture 1
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixVal); //mix texture 1 and texture 2
}
