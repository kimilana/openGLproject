#include "Mouse.h"

//static variables
double Mouse::x = 0; 
double Mouse::y = 0; 

double Mouse::lastX = 0; 
double Mouse::lastY = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

double Mouse::scrollDX = 0;
double Mouse::scrollDY = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 }; //GLFW_MOUSE_BUTTON_LAST is a number representing the number of buttons
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = {0}; 

//when this callback method is called, it evaluates the x and y positon of the mouse cursor as well as the change in x and y
void Mouse::cursorPosCallback(GLFWwindow* window, double _x, double _y) {
	x = _x; 
	y = _y; 

	if (firstMouse) { //mouse has not been moved yet
		lastX = x; 
		lastY = y; 
		firstMouse = false; 
	}

	dx = x - lastX;
	dy = lastY - y; //inverted because the top left is (0,0) 
	lastX = x; 
	lastY = y; 
}

//mouse buttons 
void Mouse::mouseButtonCallback(GLFWwindow*, int button, int action, int mods) {
	if (action != GLFW_RELEASE) { //if the button is being pressed or held down
		if (!buttons[button]) { //if the button is not already pressed
			buttons[button] = true; //set the state of the button in the array to "true" meaning that it is being pressed down
		}
	}
	else {
		buttons[button] = false; //button has been released 
	}
	buttonsChanged[button] = action != GLFW_REPEAT; //true if button is pressed or released  
}

void Mouse::mouseWheelCallback(GLFWwindow* window, double dx, double dy) {
	scrollDX = dx;
	scrollDY = dy; 
}


//returns the x position of the mouse cursor
double Mouse::getMouseX() {
	return x; 
}

//returns the y position of the mouse cursor
double  Mouse::getMouseY() {
	return y; 
}


//returns the amount the x position of the mouse has changed when the cursor is moved  
double  Mouse::getDX() {
	double _dx = dx;
	dx = 0; //dx resets each frame 
	return _dx;
}

//returns the amount the y position of the mouse has changed when the cursor is moved  
double  Mouse::getDY() {
	double _dy = dy;
	dy = 0; //dy resets each frame 
	return _dy; 
}

//returns the amount the x coordinate is changed by the scroll wheel 
double  Mouse::getScrollDX() {
	double dx = scrollDX; 
	scrollDX = 0; 
	return dx; 
}

//returns the amount the y coordinate is changed by the scroll wheel 
double  Mouse::getScrollDY() {
	double dy = scrollDY;
	scrollDY = 0;
	return dy;
}


//returns true if the button is down 
bool  Mouse::button(int button) {
	return buttons[button]; 

}

//returns true if the state of the mouse button was just changed
bool  Mouse::buttonChanged(int button) {
	bool ret = buttonsChanged[button];
	buttonsChanged[button] = false; 
	return ret; 

}

bool  Mouse::buttonWentUp(int button) {
	return !buttons[button] && buttonChanged(button); 
}

bool  Mouse::buttonWentDown(int button) {
	return buttons[button] && buttonChanged(button); 
}