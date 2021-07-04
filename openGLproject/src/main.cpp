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

#include "Shader.h" 

#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Joystick.h"

 
//method declaration

// framebuffer size function takes a GLFWwindow as its first argument and two integers indicating the new window dimensions. 
// Whenever the window changes size, GLFW calls this function and fills in the proper arguments for you to process
//pass in 3 parameters: a pointer to your GLFWwindow, width, and height 
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

//method to process user input
void processInput(GLFWwindow* window);


//global variables
float mixVal = 0.5f; //variable to control the mixing of textures through the shader
float ww = 800; 
float wh = 600; 

glm::mat4 transform = glm::mat4(1.0f); 
Joystick mainJ(0); 





int main() {

    glfwInit(); //initializes glfw

    // openGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //pass hints to the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 

    //use core profile for GLFW
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //initialize window object 
    GLFWwindow* window = glfwCreateWindow(ww, wh, "Window", NULL, NULL); //create a pointer to our window, specify width, height, title, monitor(null)
     
    //check to see if window was created by checking if the pointer is null
    if (window == NULL) { //window not created
        std::cout << "could not create window." << std::endl;
        glfwTerminate(); //this function terminates glfw
        return -1; 
    }

    //set the focus of GLFW to our window
    glfwMakeContextCurrent(window); 

    //glad code 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //Glad didn't initialize
    std::cout << "Failed to initialize GLAD" << std::endl; 
    glfwTerminate();
    return -1;
}

    //set viewport. Tell OpenGL how to display/ how to render data
    // processed coordinates in OpenGL are between -1 and 1, so we effectively map from the range (-1,1) to our window's coordinates, for example (0,800) and (0,600)
    glViewport(0, 0, ww, wh); //set the position and dimensions of the window, (x position of lower left corner y position of lower left corner, window width,window height) 

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

    glfwSetKeyCallback(window, Keyboard::keyCallback); //sets the key callback of the window which is called when a key is pressed, released or held. 
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback); //sets the mouse position callback of the window 
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback); 




    /*
        shaders
    */
    Shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");
    Shader shader2("assets/vertex_core.glsl", "assets/fragment_core2.glsl");
    Shader mandelbrotShader("assets/vertex_core.glsl", "assets/fragment_mandelbrot.glsl");


    //vertex array
    float vertices[] = { //vertex data 

        //positions             colors                      texture coordinates      
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f, 0.5f,           1.0f, 1.0f,    //top right
        -0.5f, 0.5f, 0.0f,      0.5f, 1.0f, 0.75f,          0.0f, 1.0f,    //top left
        -0.5f, -0.5f, 0.0f,     0.6f, 1.0f, 0.2f,           0.0f, 0.0f,    //bottom left
        0.5f, -0.5f, 0.0f,      1.0f, 0.2f, 1.0f,           1.0f, 0.0f     //bottom right


    };

    //integer array of indices for our triangles
    unsigned int indices[] = {
        0, 1, 2, //first triangle
        2, 3, 0 //second triangle
    }; 

    //VAO, VBO
    unsigned int VAO, VBO, EBO; 
    glGenVertexArrays(1, &VAO); //Vertex array object (VAO), contains pointers to the vertex buffers
    glGenBuffers(1, &VBO); //Vertex buffer object (VBO), buffer containining vertex data 
    glGenBuffers(1, &EBO); //element buffer object

    //bind VAO
    glBindVertexArray(VAO); // passes in an integer and openGL will know which array object is the active one

    //bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //static draw tells openGL that we won't be modifying this data much


    //put index array in EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind our EBO 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 


    //set attributes pointers (the attributes here are vertex positions, vertex color, and texture coordinates)   

    //positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // (index, number of values per vertex, type of data, bool normalized, size, offset)
    glEnableVertexAttribArray(0); //index=0 

    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); 
    glEnableVertexAttribArray(1); //index=1

    //texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); 
    glEnableVertexAttribArray(2); 


   /*
   * Textures
   */

    unsigned int texture1, texture2; 

    glGenTextures(1, &texture1); //creates a texture
    glBindTexture(GL_TEXTURE_2D, texture1); //tells openGL that texture1 is the active texture. On the GPU, the first texture unit points to texture1   

    //integer parameters for textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //x coordinate
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //y coordinate

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    //load image
    int width, height, nChannels; 
    stbi_set_flip_vertically_on_load(true); //stbi loads images upside down, so we flip them vertically to make them rightside up
    unsigned char* data = stbi_load("assets/greyhounds.jpg", &width, &height, &nChannels, 0); //holds the data from our image file (the variable "data" is a pointer to the specified image data in memory 

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //specify a two-dimensional texture image. (target texture, level-of-detail number, number of color components, width, height, border (must be 0), format of pixel data, type of pixel data, data (specifies a pointer to the image data in memory) 
        glGenerateMipmap(GL_TEXTURE_2D); //mipmaps are pre-calculated, optimized sequences of images, each of which is a progressively lower resolution representation of the previous.
    
    }
    else { //if the texture didn't load 
        std::cout << "Failed to load texture" << std::endl; 
    }

    stbi_image_free(data); //free the loaded image from memory

   
    glGenTextures(1, &texture2); 
    glBindTexture(GL_TEXTURE_2D, texture2); 

    data = stbi_load("assets/greyhound2.jpg", &width, &height, &nChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);


    shader.activate(); 
    shader.setInt("texture1", 0); //the shader will look for texture unit 0 
    shader.setInt("texture2", 1); 


    //matrix translation to rotate the shape

   
    glm::mat4 trans = glm::mat4(1.0f); //initializes identity matrix

     /*
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //rotate the identity matrix
    shader.activate(); 
    shader.setMat4("transform", trans); 
   

   
    glm::mat4 trans2 = glm::mat4(1.0f);
    trans2 = glm::scale(trans2, glm::vec3(1.5f));
    trans2 = glm::rotate(trans2, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    shader2.activate();
    shader2.setMat4("transform", trans); 
    */

    //determine if a joystick is present
    mainJ.update(); 
    if (mainJ.isPresent()) {
        std::cout << mainJ.getName() << " is present." << std::endl; 
    }
    else {
        std::cout << "Not present." << std::endl; 
    }


    //while loop that runs the program
    while (!glfwWindowShouldClose(window)) {
        //process input
        processInput(window);

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT); 

        glActiveTexture(GL_TEXTURE0); //activate 0th texture unit
        glBindTexture(GL_TEXTURE_2D, texture1); //bind the texture to the active unit. Tell the 0th texture unit to point to texture 1 which points to the image data 

        glActiveTexture(GL_TEXTURE1); //for the second image
        glBindTexture(GL_TEXTURE_2D, texture2);


         //clear the entire window and set color to specified color in RGBA format
        //glClearColor(0.1f, 0.3f, 0.3f, 1.0f); //blue background color 

        glClearColor(1.0f, 0.7f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        //trans = glm::rotate(trans, glm::radians(0.05f), glm::vec3(0.1, 0.1f, 0.1f));
        shader.activate(); 
        //shader.setMat4("transform", trans); 
        //shader2.activate();
        //shader2.setMat4("transform", trans);

        shader.setFloat("mixVal", mixVal); 
        mandelbrotShader.setMat4("transform", transform); 

        // draw shapes
        glBindVertexArray(VAO); //openGL now knows which vertex array object to look at, and as a result knows which vertex buffer data to look at
        shader.activate();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0); 

        //trans2 = glm::rotate(trans2, glm::radians((float)glfwGetTime() / -100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //shader2.activate(); 
        //shader2.setMat4("transform", trans2); 
        //shader2.activate();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint))); //we change the offset by 3 indices



        // send new frame to window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    glfwTerminate();
    return 0;
}







//defines the method framebuffer_size_callback 
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //reset the glViewport every time it is resized 
    //windowWidth = width; 
    //windowHeight = height; 
    std::cout << width << std::endl;
    std::cout << height << std::endl; 
}

//defines the method processInput

void processInput(GLFWwindow* window) {
    if (Keyboard::key(GLFW_KEY_ESCAPE) || mainJ.buttonState(GLFW_JOYSTICK_BTN_RIGHT)) { //test if escape key is pressed down
        glfwSetWindowShouldClose(window, true); //this will end the while loop, closing the window if the escape key is pressed 
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


    //use ASDW keys to move the image around the screen

    
    if (Keyboard::key(GLFW_KEY_W)) { //if W key is pressed 
        transform = glm::translate(transform, glm::vec3(0.0f, 0.001f, 0.0f)); //matrix transform to move in the positive y direction 
    }

    if (Keyboard::key(GLFW_KEY_S)) { //if S key is pressed 
        transform = glm::translate(transform, glm::vec3(0.0f, -0.001f, 0.0f)); //matrix transform to move in the positive y direction 
    }

    if (Keyboard::key(GLFW_KEY_A)) { //if A key is pressed 
        transform = glm::translate(transform, glm::vec3(-0.001f, 0.0f, 0.0f)); //matrix transform to move in the negative x direction
    }

    if (Keyboard::key(GLFW_KEY_D)) { //if D key is pressed 
        transform = glm::translate(transform, glm::vec3(0.001f, 0.0f, 0.0f)); //matrix transform to move in the positive x direction
    }

    if (Keyboard::key(GLFW_KEY_Z)) {
        transform = glm::scale(transform, glm::vec3(1.001, 1.001, 0.0f));
    }

    if (Keyboard::key(GLFW_KEY_X)) {
        transform = glm::scale(transform, glm::vec3(.999, .999, 0.0f));
        
    }
   
   

  /*
    mainJ.update(); 

    float lx = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_X); 
    float ly = -mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_Y); 

    if (std::abs(lx) > 0.05f) { //deadzone: have to move joystick a certain amount to move 
        transform = glm::translate(transform, glm::vec3(lx / 1000, 0.0f, 0.0f));   
    }

    if (std::abs(ly) > 0.05f) {
        transform = glm::translate(transform, glm::vec3(0.0f, ly / 1000, 0.0f));
    }


   //right joystick trigger zooms in. the triggers are axes because they represent a continuous range of values 
    float rt = mainJ.axesState(GLFW_JOYSTICK_AXES_RIGHT_TRIGGER) / 2 + 0.5f;  //normal range for triggers is [-1,1], we are switching it to [0,1]

    //left joystick trigger zooms out 
    float lt = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_TRIGGER) / 2 + 0.5f;
    if (lt < 0.500008f) {
        transform = glm::scale(transform, glm::vec3(1-lt / 10, 1-lt / 10, 0.0f));
    }
    if (lt > 0.500008f) {
        transform = glm::scale(transform, glm::vec3(1 + lt / 10, 1 + lt / 10, 0.0f));
    }

    std::cout << "LT" << lt << std::endl;
    std::cout << "RT" << rt << std::endl;
      
   */


}


