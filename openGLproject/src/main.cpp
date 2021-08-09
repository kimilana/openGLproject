#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include "graphics/Shader.h" 
#include "graphics/Texture.h"

#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Joystick.h"
#include "io/Camera.h"
#include "io/Screen.h"

 
//method declaration

// framebuffer size function takes a GLFWwindow as its first argument and two integers indicating the new window dimensions. 
// Whenever the window changes size, GLFW calls this function and fills in the proper arguments for you to process
//pass in 3 parameters: a pointer to your GLFWwindow, width, and height 
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

//method to process user input
void processInput(double dt); //dt is deltaTime

//global variables
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

Screen screen; 

float mixVal = 0.6f; //variable to control the mixing of textures through the shader

glm::mat4 mouseTransform = glm::mat4(1.0f); 

//cameras 
Camera cameras[2] = {
    Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
    Camera(glm::vec3(10.0f, 10.0f, 10.0f))
}; 
int activeCam = 0; 
float deltaTime = 0.0f; 
float lastFrame = 0.0f; 

//joystick
Joystick mainJ(0);



int main() {

    glfwInit(); //initializes glfw

    // openGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //pass hints to the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 

    //use core profile for GLFW
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

# ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

    //create window or give error message if window creation fails
    if (!screen.init()) {
        std::cout << "could not create window." << std::endl;
        glfwTerminate(); //this function terminates glfw
        return -1;
    }

    //glad code 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //Glad didn't initialize
    std::cout << "Failed to initialize GLAD" << std::endl; 
    glfwTerminate();
    return -1;
}

    screen.setParameters(); 

   

    /*
        shaders
    */
    Shader shader("assets/object.vs", "assets/object.fs");
    Shader mandelbrotShader("assets/object.vs", "assets/fragment_mandelbrot.glsl");

    //VERTICES FOR A CUBE

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    /* VERTICES OF A RECTANGLE
    //vertex array
    float vertices[] = { //vertex data 

        //positions             colors                      texture coordinates      
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f, 0.5f,           1.0f, 1.0f,    //top right
        -0.5f, 0.5f, 0.0f,      0.5f, 1.0f, 0.75f,          0.0f, 1.0f,    //top left
        -0.5f, -0.5f, 0.0f,     0.6f, 1.0f, 0.2f,           0.0f, 0.0f,    //bottom left
        0.5f, -0.5f, 0.0f,      1.0f, 0.2f, 1.0f,           1.0f, 0.0f     //bottom right


    };

    //integer array of indices for our triangles. We will put this data in the EBO
    unsigned int indices[] = {
        0, 1, 2, //first triangle
        2, 3, 0 //second triangle
    }; 

    */

    //VAO, VBO
    unsigned int VAO, VBO; 
    glGenVertexArrays(1, &VAO); //Vertex array object (VAO), contains pointers to the vertex buffers
    glGenBuffers(1, &VBO); //Vertex buffer object (VBO), buffer containining vertex data 

    //bind VAO
    glBindVertexArray(VAO); // passes in an integer and openGL will know which array object is the active one

    //bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //static draw tells openGL that we won't be modifying this data much


    //set attributes pointers (the attributes here are vertex positions, vertex color, and texture coordinates)   

    //positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*)0); // (index, number of values per vertex, type of data, bool normalized, size, offset)
    glEnableVertexAttribArray(0); //index=0 

    //color
    //VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); 
    //EnableVertexAttribArray(1); //index=1

    //texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); 
    glEnableVertexAttribArray(1); 


   /*
   * Textures
   */

    Texture texture1("assets/marylandflag.jpg", "texture1");
    texture1.load();
    Texture texture2("assets/crab.jpg", "texture2"); 
    texture2.load(); 

    shader.activate(); 
    shader.setInt("texture1", texture1.id); //the texture unit is specified by the id variable
    shader.setInt("texture2", texture2.id); 


    //determine if a joystick is present
    mainJ.update(); 
    if (mainJ.isPresent()) {
        std::cout << mainJ.getName() << " is present." << std::endl; 
    }
    else {
        std::cout << "Joystick not present." << std::endl; 
    }




    /*
     WHILE LOOP THAT RUNS THE PROGRAM
    
    */
    while (!screen.shouldClose()) {
        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime; 

        //process input
        processInput(deltaTime);
        screen.update();

        //bind textures
        glActiveTexture(GL_TEXTURE0); //activate 0th texture unit
        texture1.bind(); //bind the texture to the active unit. Tell the 0th texture unit to point to texture 1 which points to the image data 
        glActiveTexture(GL_TEXTURE1); //for the second image
        texture2.bind(); //bind the second texture

        // draw shapes
        glBindVertexArray(VAO); //openGL now knows which vertex array object to look at, and as a result knows which vertex buffer data to look at

        //create transformation for screen
        glm::mat4 model = glm::mat4(1.0f); //model transformation is for objects going from local space to world space
        glm::mat4 view = glm::mat4(1.0f); //view transformation is for objects going from view world space to view space. Specific to each camera
        glm::mat4 projection = glm::mat4(1.0f); 

        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));  //rotation
        view = cameras[activeCam].getViewMatrix(); //view matrix comes from our camera
        projection = glm::perspective(glm::radians(cameras[activeCam].getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //field of view

        shader.activate(); 

        shader.setMat4("model", model); 
        shader.setMat4("view", view); 
        shader.setMat4("projection", projection); 

        shader.setFloat("mixVal", mixVal);
        shader.setMat4("mouseTransform", mouseTransform);

        glDrawArrays(GL_TRIANGLES, 0, 36); //we have 6 faces and 6 vertices per face (4 position vertices and 2 texture vertices) so we have 36 in total

        glBindVertexArray(0); 

        // send new frame to window
        screen.newFrame();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


    glfwTerminate();
    return 0;
}




//defines the method framebuffer_size_callback 
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //reset the glViewport every time it is resized 
    SCR_WIDTH = width; 
    SCR_HEIGHT = height; 
    
}

//defines the method processInput

void processInput(double dt) {
    if (Keyboard::key(GLFW_KEY_ESCAPE) || mainJ.buttonState(GLFW_JOYSTICK_BTN_RIGHT)) { //test if escape key is pressed down
        screen.setShouldClose(true);
    }


        //change mix value
        if (Keyboard::key(GLFW_KEY_UP)) {
            mixVal += .001f;

            if (mixVal > 1) {
                mixVal = 1.0f;
            }
        }

        if (Keyboard::key(GLFW_KEY_DOWN)) {
            mixVal -= .001f;

            if (mixVal < 0) {
                mixVal = 0.0f;
            }
        }

        if (Keyboard::keyWentDown(GLFW_KEY_TAB)) {
            activeCam += (activeCam == 0) ? 1 : -1; //if the active cam is 1, add 1 otherwise subtract 1
        }

        //move camera
        if (Keyboard::key(GLFW_KEY_W)) {
            cameras[activeCam].updateCameraPos(CameraDirection::FORWARD, dt);
        }
        if (Keyboard::key(GLFW_KEY_S)) {
            cameras[activeCam].updateCameraPos(CameraDirection::BACKWARD, dt);
        }
        if (Keyboard::key(GLFW_KEY_D)) {
            cameras[activeCam].updateCameraPos(CameraDirection::RIGHT, dt);
        }
        if (Keyboard::key(GLFW_KEY_A)) {
            cameras[activeCam].updateCameraPos(CameraDirection::LEFT, dt);
        }
        if (Keyboard::key(GLFW_KEY_SPACE)) {
            cameras[activeCam].updateCameraPos(CameraDirection::UP, dt);
        }
        if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
            cameras[activeCam].updateCameraPos(CameraDirection::DOWN, dt);
        }

        //use mouse cursor to change vamera direction
        double dx = Mouse::getDX();
        double dy = Mouse::getDY();
        if (dx != 0 || dy != 0) {
            cameras[activeCam].updateCameraDirection(0.05 * dx, 0.05 * dy); //dx and dy multiplied by a factor to decrease sensitivity to mouse cha 
        }

        //use mouse scroll wheel to zoom
        double scrollDy = Mouse::getScrollDY();
        if (scrollDy != 0) {
            cameras[activeCam].updateCameraZoom(scrollDy);
        }
    }





   
   




