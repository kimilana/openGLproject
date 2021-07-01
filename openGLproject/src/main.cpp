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

 
//method declaration

// framebuffer size function takes a GLFWwindow as its first argument and two integers indicating the new window dimensions. 
// Whenever the window changes size, GLFW calls this function and fills in the proper arguments for you to process
//pass in 3 parameters: a pointer to your GLFWwindow, width, and height 
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

//method to process input
void processInput(GLFWwindow* window);

float mixVal = 0.5f; //variable to control the mixing of textures through the shader

std::string  loadShaderSrc(const char* filename); //this method will be used to load the source code of our glsl shaders 


int main() {

    int success; //success of the compilation of shader code 
    char infolog[512]; //to store error message if there is one 

    //glm test (test the translation, scale, and rotation features of the glm libraries)

    //glm::vec4 vec(1.0f, 1.0f, 1.0f, 1.0f); //original vector with homogeneous coordinate, w, set to 1
    //glm::mat4 trans = glm::mat4(1.0f); //4x4 identity matrix
    //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); // matrix that translates by (1,1,0)
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); //Rotation matrix. pass in the angle of rotation and the vector to act upon
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); //translation matrix to scale by (0.5,0.5,0.5)
    //vec = trans * vec; //translate the original vector by multiplying it by the transformation matrix
    //std::cout << vec.x  << ' ' << vec.y << ' ' << vec.z << ' ' << std::endl; //print out coordinates of transformed matrix

    glfwInit(); //initializes glfw

    // openGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //pass hints to the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 

    //use core profile for GLFW
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //initialize window object 

    GLFWwindow* window = glfwCreateWindow(800, 600, "Window", NULL, NULL); //create a pointer to our window, specify width, height, title, monitor(null)
     
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
    
    glViewport(0, 0, 800, 600); //set the position and dimensions of the window, (x position of lower left corner y position of lower left corner, window width,window height) 

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

    Shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");
    Shader shader2("assets/vertex_core.glsl", "assets/fragment_core2.glsl");

    //vertex array
    float vertices[] = { //vertex data 

        //positions             colors                      texture coordinates      
        0.7f, 0.7f, 0.0f,       1.0f, 1.0f, 0.5f,           1.0f, 1.0f,    //top right
        -0.7f, 0.7f, 0.0f,      0.5f, 1.0f, 0.75f,          0.0f, 1.0f,    //top left
        -0.7f, -0.7f, 0.0f,     0.6f, 1.0f, 0.2f,           0.0f, 0.0f,    //bottom left
        0.7f, -0.7f, 0.0f,      1.0f, 0.2f, 1.0f,           1.0f, 0.0f     //bottom right


    };

    //integer array
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //static draw teells openGL that we won't be modifying this data much


    //put index array in EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //set attributes pointers

    //positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // (index, number of values per vertex, type of data, bool normalized, size,offset)
    glEnableVertexAttribArray(0); //index=0 

    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); 
    glEnableVertexAttribArray(1); //index=1

    //texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); 
    glEnableVertexAttribArray(2); 


    // TEXTURES

    unsigned int texture1, texture2; 

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //x coordinate
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //y coordinate

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //load image
    int width, height, nChannels; 
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* data = stbi_load("assets/greyhounds.jpg", &width, &height, &nChannels, 0); 

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    
    }
    else {
        std::cout << "Failed to load texture" << std::endl; 
    }

    stbi_image_free(data); 

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
}

//defines the method processInput

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { //test if escape key is pressed down
        glfwSetWindowShouldClose(window, true); //this will end the while loop, closing the window if the escape key is pressed 
    }


    //change mix value
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixVal += .001f; 

        if (mixVal > 1) {
            mixVal = 1.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixVal -= .001f;
        
        if (mixVal < 0) {
            mixVal = 0.0f;
        }
    }

}


