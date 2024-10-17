
#include "../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>



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

int testCompilationShader(unsigned int vertexShader){
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }
}

float* returnTringlePoints(){
    // Выделяем память под 9 float-ов
    float* vertices = (float*)malloc(9 * sizeof(float));
    if (vertices == NULL) {
        // Если выделение памяти не удалось
        return NULL;
    }

    // Заполняем массив цветами
    vertices[0] = -0.5f; vertices[1] = -0.5f; vertices[2] = 0.0f;
    vertices[3] =  0.5f; vertices[4] = -0.5f; vertices[5] = 0.0f;
    vertices[6] =  0.0f; vertices[7] =  0.5f; vertices[8] = 0.0f;


    return vertices;  // Возвращаем указатель на выделенную память
}

float* returnTringleCollor(){
    // Выделяем память под 9 float-ов
    float* vertices = (float*)malloc(9 * sizeof(float));
    if (vertices == NULL) {
        // Если выделение памяти не удалось
        return NULL;
    }

    // Заполняем массив координатами
    vertices[0] = 1.0f; vertices[1] = 0.0f; vertices[2] = 0.0f;
    vertices[3] = 0.0f; vertices[4] = 1.0f; vertices[5] = 0.0f;
    vertices[6] = 0.0f; vertices[7] = 0.0f; vertices[8] = 1.0f;

    return vertices;  // Возвращаем указатель на выделенную память
}

//---------------------------------SHADERS---------------------------------
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main(){\n"
    "   ourColor = aColor;\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "in vec3 ourColor;\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}\0";

        
unsigned int shaderTransferAndUnion(){
    //vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //union of shaders (linking)
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    testCompilationShader(vertexShader);

    //delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

unsigned int shaderTransfear(){
    
    //------------------transfare shaders to gpu------------------
    //ponits
    unsigned int pointsVBO;
    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO); //make VBO active (my buffer)
    float* verticesPoint = returnTringlePoints();
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), verticesPoint, GL_STATIC_DRAW); //give to gpu data of positions

    //colors
    unsigned int colorsVBO;
    glGenBuffers(1, &colorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO); //make VBO active (my buffer)
    float* verticesColor = returnTringleCollor();
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), verticesColor, GL_STATIC_DRAW); //give to gpu data of collors

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

}

void rendering(GLFWwindow* window){//VBO vertex buffer object VAO vertex atrributes object

    unsigned int shaderProgram = shaderTransferAndUnion();
    unsigned int VAO = shaderTransfear();

    while(!glfwWindowShouldClose(window)){
        //inputs
        processInput(window);  
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        //rendering commands here
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);//use shader
        glBindVertexArray(VAO);//use vertex atrributes object #10 objects 10 vao
        glDrawArrays(GL_TRIANGLES, 0, 3); //draw VAO

        //double buffering
        glfwSwapBuffers(window);
        //resaved and then return imeadiately
        glfwPollEvents();    
    }
}
 

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
    shaderTransferAndUnion();
    shaderTransfear(); 
    rendering(window);

    glfwTerminate();

    return 0;
}