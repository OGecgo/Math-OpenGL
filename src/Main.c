
#include "../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "renderer/ShaderProgram.h"
#include "../res/ResourceManager.h"

void properties();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void rendering(GLFWwindow* window);


int main(){

    properties();
    GLFWwindow* window = glfwCreateWindow(1330, 630, "Shaders", NULL, NULL);
    if (window == NULL){
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Failed to initialize GLAD\n");
        return -1;
    }  

    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
    rendering(window);

    destructionShaderProgram();
    glfwTerminate();

    return 0;
}

void properties(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//core-profile
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}


int points = 5;
int tringles = 9;

float* returnTringlePoints(){
    float* vertices = (float*)malloc((points * 3) * sizeof(float));
    if (vertices == NULL) return NULL;
    //x, y, z
    vertices[0] = 0.5f; vertices[1] = 0.25f; vertices[2] = 0.0f;
    vertices[3] = 0.5f; vertices[4] = -0.25f; vertices[5] = 0.0f;
    vertices[6] = -0.5f; vertices[7] =  -0.25f; vertices[8] = 0.0f;

    vertices[9] =  -0.5f; vertices[10] = 0.25f; vertices[11] = 0.0f;

    vertices[12] = 0.0f; vertices[13] = 0.5f; vertices[14] = 0.0f;

    return vertices;
}

float* returnTringleCollor(){
    float* vertices = (float*)malloc((points * 3) * sizeof(float));
    if (vertices == NULL) return NULL;

    vertices[0] = 1.0f; vertices[1] = 0.0f; vertices[2] = 0.0f;
    vertices[3] = 0.0f; vertices[4] = 0.0f; vertices[5] = 0.0f;
    vertices[6] = 0.0f; vertices[7] = 0.0f; vertices[8] = 0.0f;

    vertices[9] = 1.0f; vertices[10] = 1.0f; vertices[11] = 1.0f;

    vertices[12] = 1.0f; vertices[13] = 1.0f; vertices[14] = 1.0f;

    return vertices;
}

unsigned int* returnTringleIndex(){
    unsigned int* index = (unsigned int*)malloc(tringles * sizeof(unsigned int));
    if (index == NULL) return NULL;

    index[0] = 0; index[1] = 1; index[2] = 2;
    index[3] = 2; index[4] =3; index[5] = 0;
    index[6] = 3; index[7] = 4; index[8] = 0;

    return index;
}

//---------------------------------SHADERS---------------------------------



void rendering(GLFWwindow* window){//VBO vertex buffer object VAO vertex atrributes object EBO element buffer object
    //------------------shaders------------------
    char* vertexShaderSource = returnShaderSource("res/shaders/vertex.glsl");
    char* fragmentShaderSource = returnShaderSource("res/shaders/fragment.glsl");

    ShaderProgram(vertexShaderSource, fragmentShaderSource);

    //------------------transfare shaders to gpu------------------
    //ponits
    unsigned int pointsVBO;
    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO); //make VBO active (my buffer)
    float* verticesPoint = returnTringlePoints();
    glBufferData(GL_ARRAY_BUFFER, (points * 3) * sizeof(float), verticesPoint, GL_STATIC_DRAW); //give to gpu data of positions

    //colors
    unsigned int colorsVBO;
    glGenBuffers(1, &colorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO); //make VBO active (my buffer)
    float* verticesColor = returnTringleCollor();
    glBufferData(GL_ARRAY_BUFFER, (points * 3) * sizeof(float), verticesColor, GL_STATIC_DRAW); //give to gpu data of collors

    //------------------data processing shaders to gpu------------------
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); //make VAO active (my vertex) 

    glEnableVertexAttribArray(0); //location 0
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); //make data

    glEnableVertexAttribArray(1); //location 1
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL); //make data

    //------------------indexing bufers------------------
    unsigned int EBO; 
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
    unsigned int* index = returnTringleIndex();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tringles * sizeof(unsigned int), index, GL_STATIC_DRAW); //give to gpu data of positions

    while(!glfwWindowShouldClose(window)){
        //inputs
        processInput(window);  
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        //rendering commands here
        glClear(GL_COLOR_BUFFER_BIT);

        useShaderProgram();//use shader
        glBindVertexArray(VAO);//use vertex atrributes object #10 objects 10 vao
        //glDrawArrays(GL_TRIANGLES, 0, 3); //draw VAO
        glDrawElements(GL_TRIANGLES, tringles, GL_UNSIGNED_INT, 0); //draw with EBO
        glBindVertexArray(0);//unbind VAO

        //double buffering
        glfwSwapBuffers(window);
        //resaved and then return imeadiately
        glfwPollEvents();    
    }
}
 

