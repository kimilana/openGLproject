#version 330 core
out vec4 FragColor; //output a 4D vector called FragColor. Because it's coming from the fragment shader OpenGL knows it will be the color of the fragment


void main() {
	FragColor = vec4(0.5f, 0.82f, 0.11f, 1.0f);  //green
	}