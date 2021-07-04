#version 330 core
out vec4 FragColor; //output a 4D vector called FragColor. Because it's coming from the fragment shader OpenGL knows it will be the color of the fragment

vec2 squareImaginary(vec2 num) {
	return vec2(
		pow(num.x,2) - pow(num.y,2), 
		2 * num.x * num.y
	); 
}

float iterateMandelbrot(vec2 coord, int maxIterations) {
	vec2 z = vec2(0,0);
	for(int i=0; i < maxIterations; i++) {
		z = squareImaginary(z) + coord; 
		
		if(length(z) > 2 ) 
			return (1.0-(i/100.0));
	}
	return 0.0;  //inner color
}


void main() {

	float screen_width = 800;
	float screen_height = 600; 

	vec2 pos = gl_FragCoord.xy; 
	vec2 center = vec2(screen_width/2.0, screen_height/2.0);
	vec2 newpos = ((pos - center + vec2(-100.0,0.0)) * (0.01)) * (0.3); 

	float col = iterateMandelbrot(newpos, 100); 

	FragColor = vec4(col, col, col, 1.0f); 

	}

