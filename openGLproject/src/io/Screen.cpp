#include "screen.h"

#include "keyboard.h"
#include "mouse.h"

unsigned int Screen::SCR_WIDTH = 800; 
unsigned int Screen::SCR_HEIGHT = 600; 

void Screen::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); 

	SCR_WIDTH = width; 
	SCR_HEIGHT = height;
}

//constructor
Screen::Screen()
	: window(nullptr) {}

//create window and return true if the window was created successfully
bool Screen::init() {
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window", NULL, NULL); 
	if (window == NULL) { //window not created
		return false; 
	}

	glfwMakeContextCurrent(window); 
	return true; 
}

void Screen::setParameters() {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, Screen::framebufferSizeCallback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //do not show cursor
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback); 
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback); 

	glEnable(GL_DEPTH_TEST); 
}

void Screen::update() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void Screen::newFrame() {
	glfwSwapBuffers(window); 
	glfwPollEvents();
}

bool Screen::shouldClose() {
	return glfwWindowShouldClose(window); 
}

void Screen::setShouldClose(bool shouldClose) {
	glfwSetWindowShouldClose(window, shouldClose); 
}