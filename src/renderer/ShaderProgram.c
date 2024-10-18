
#include "ShaderProgram.h"
#include "../../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int shaderProgram = 0;

//create shader (vertex or fragment)
int CreateShader(GLenum shaderType, const char* shaderSource, unsigned int *shaderID){
    *shaderID = glCreateShader(shaderType);
    glShaderSource(*shaderID, 1, &shaderSource, NULL);
    glCompileShader(*shaderID);

    int  success;
    char infoLog[512];
    glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(*shaderID, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        return 1;
    }
    return 0;
}

void ShaderProgram(char *vertexShaders, char *fragmantShaders){
    //vertex shader
    unsigned int vertexShader;
    if (CreateShader(GL_VERTEX_SHADER, vertexShaders, &vertexShader)) {
        return; // Exit if vertex shader compilation failed
    }
    
    //fragment shader
    unsigned int fragmentShader;
    if (CreateShader(GL_FRAGMENT_SHADER, fragmantShaders, &fragmentShader)) {
        glDeleteShader(vertexShader); // Clean up vertex shader if fragment shader compilation failed
        return;
    }

    //union of shaders (linking)
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success){
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    //delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void destructionShaderProgram(){
    glDeleteProgram(shaderProgram);
}

void useShaderProgram(){
    glUseProgram(shaderProgram);
}





