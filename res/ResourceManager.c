#include <stdio.h>
#include <stdlib.h>

#include "ResourceManager.h"

char* returnShaderSource(char* filePos){
    FILE* file = fopen(filePos, "r");
    if (file == NULL){
        printf("Failed to open Shader.glsl\n%s\n", filePos);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char* shaderSource = (char*)malloc(fileSize + 1); //+1 for null terminator

    fread(shaderSource, 1, fileSize, file);
    shaderSource[fileSize] = '\0'; //add null terminator

    fclose(file);
    return shaderSource;

}