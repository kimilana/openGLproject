#include "Keyboard.h"
//private methods
bool Keyboard::keys[GLFW_KEY_LAST] = { 0 }; // state array of keys. GLFW_KEY_LAST is defined in the header file of GLFW. Initialize all to 0. 
bool Keyboard::keysChanged[GLFW_KEY_LAST] = { 0 }; 

//public methods

//keeps track of if a key is being pressed. keys can be in three states: GLFW press, release, and repeat. Repeat means the key is being held down, hence the use of !release.  
void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_RELEASE) {
		if (!keys[key]) {
			keys[key] = true; 
		}
	} 
	else {
		keys[key] = false;
	}

	keysChanged[key] = action != GLFW_REPEAT;  //if the key is pressed or released (if it is not being held down), set the state of keysChanged to true. 
}

 // return the state currently stored in the state array 
bool Keyboard::key(int key) {
	return keys[key]; 

}


bool Keyboard::keyChanged(int key) {
	bool ret = keysChanged[key]; //store the value that will be returned by this method
	keysChanged[key] = false; //set it to false so it does not get changed every frame, only in the current frame
	return ret; 
}

bool Keyboard::keyWentUp(int key) {
	return !keys[key] && keyChanged(key); //determine whether the key is currently up and if it was recently changed
}

bool Keyboard::keyWentDown(int key) {
	return keys[key] && keyChanged(key);
}