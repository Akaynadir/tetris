#include "terrain.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <algorithm>
#include "cube.h"
#include <iostream>


Terrain::Terrain(int w, int h) : width(w), height(h) {
    grid.resize(width);
    for (int i = 0; i < width; i++) {
        grid[i].resize(height, false);
    }
}

void Terrain::drawBorders(Cube& cube, GLuint shaderProgram) {
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");

    glm::vec3 borderColor(0.3f, 0.3f, 0.35f);
    Cube borderCube(borderColor);

    for (int y = 0; y <= height; y++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.5f, y, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 1.0f, 0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        borderCube.draw();
    }

    for (int y = 0; y <= height; y++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(width - 0.5f, y, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 1.0f, 0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        borderCube.draw();
    }

    for (int x = 0; x < width; x++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 0.1f, 0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        borderCube.draw();
    }

    for (int x = 0; x < width; x++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, height - 0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 0.1f, 0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        borderCube.draw();
    }
}


void Terrain::drawGrid(Cube& cube, GLuint shaderProgram) {
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");

    glm::vec3 gridColor(0.15f, 0.15f, 0.2f);
    Cube gridCube(gridColor);

    for (int x= 0; x <= width; x++) {
        for (int y=0; y< height; y++) {
            glm::mat4 model = glm::mat4(1.0f);
            model=glm::translate(model, glm::vec3(x - 0.5f, y, -0.5f));
            model=glm::scale(model, glm::vec3(0.02f, 1.0f, 0.02f));  // Très fine
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            gridCube.draw();
        }
    }

    for (int y=0; y <=height;y++) {
        for (int x=0; x< width; x++) {
            glm::mat4 model = glm::mat4(1.0f);
            model=glm::translate(model, glm::vec3(x, y - 0.5f, -0.5f));
            model=glm::scale(model, glm::vec3(1.0f, 0.02f, 0.02f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            gridCube.draw();
        }
    }
}


void Terrain::drawPlacedCubes(Cube& cube, GLuint shaderProgram) {
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");

    for (const auto& cubeData : placedCubes) {
        Cube coloredCube(cubeData.color);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(cubeData.x, cubeData.y, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        coloredCube.draw();
    }
}










