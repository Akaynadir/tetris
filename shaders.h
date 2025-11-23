#pragma once
#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>

//vertex shader
//ce shader reçoit les positions 3D et les transforme pour l'écran
const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vertexColor = aColor;
}
)";

//fragment shader
const char* fragmentShaderSource = R"(
#version 330 core

in vec3 vertexColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

//compilatiion
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);

    //compiler
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        return 0;
    }

    return shader;
}

//création du programme shader
GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    if (vertexShader == 0 || fragmentShader == 0) {
        return 0;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

#endif
