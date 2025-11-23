#include "cube.h"

Cube::Cube(glm::vec3 color) {
    createCube(color);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Cube::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);  // 36 sommets = 12 triangles = 6 faces
    glBindVertexArray(0);
}


void Cube::createCube(glm::vec3 color) {
    float vertices[] = {
        //FACE AVANT (z =0.5)
        -0.5f, -0.5f,  0.5f,  color.r, color.g, color.b,  // Bas gauche
        0.5f, -0.5f,  0.5f,  color.r, color.g, color.b,  // Bas droite
        0.5f,  0.5f,  0.5f,  color.r, color.g, color.b,  // Haut droite
        0.5f,  0.5f,  0.5f,  color.r, color.g, color.b,  // Haut droite
        -0.5f,  0.5f,  0.5f,  color.r, color.g, color.b,  // Haut gauche
        -0.5f, -0.5f,  0.5f,  color.r, color.g, color.b,  // Bas gauche

        //FACE ARRIèRE (z= -0.5)
        -0.5f, -0.5f, -0.5f,  color.r * 0.8f, color.g * 0.8f, color.b * 0.8f,
        0.5f, -0.5f, -0.5f,  color.r * 0.8f, color.g * 0.8f, color.b * 0.8f,
        0.5f,  0.5f, -0.5f,  color.r * 0.8f, color.g * 0.8f, color.b * 0.8f,
        0.5f,  0.5f, -0.5f,  color.r * 0.8f, color.g * 0.8f, color.b * 0.8f,
        -0.5f,  0.5f, -0.5f,  color.r * 0.8f, color.g * 0.8f, color.b * 0.8f,
        -0.5f, -0.5f, -0.5f,  color.r * 0.8f, color.g * 0.8f, color.b * 0.8f,

        //FACE GAUCHE (x= -0.5)
        -0.5f,  0.5f,  0.5f,  color.r * 0.6f, color.g * 0.6f, color.b * 0.6f,
        -0.5f,  0.5f, -0.5f,  color.r * 0.6f, color.g * 0.6f, color.b * 0.6f,
        -0.5f, -0.5f, -0.5f,  color.r * 0.6f, color.g * 0.6f, color.b * 0.6f,
        -0.5f, -0.5f, -0.5f,  color.r * 0.6f, color.g * 0.6f, color.b * 0.6f,
        -0.5f, -0.5f,  0.5f,  color.r * 0.6f, color.g * 0.6f, color.b * 0.6f,
        -0.5f,  0.5f,  0.5f,  color.r * 0.6f, color.g * 0.6f, color.b * 0.6f,

        //FACE DROITE (x= 0.5)
        0.5f,  0.5f,  0.5f,  color.r * 0.7f, color.g * 0.7f, color.b * 0.7f,
        0.5f,  0.5f, -0.5f,  color.r * 0.7f, color.g * 0.7f, color.b * 0.7f,
        0.5f, -0.5f, -0.5f,  color.r * 0.7f, color.g * 0.7f, color.b * 0.7f,
        0.5f, -0.5f, -0.5f,  color.r * 0.7f, color.g * 0.7f, color.b * 0.7f,
        0.5f, -0.5f,  0.5f,  color.r * 0.7f, color.g * 0.7f, color.b * 0.7f,
        0.5f,  0.5f,  0.5f,  color.r * 0.7f, color.g * 0.7f, color.b * 0.7f,

        //FACE BAS (y= -0.5)
        -0.5f, -0.5f, -0.5f,  color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,
        0.5f, -0.5f, -0.5f,  color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,
        0.5f, -0.5f,  0.5f,  color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,
        0.5f, -0.5f,  0.5f,  color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,
        -0.5f, -0.5f,  0.5f,  color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,
        -0.5f, -0.5f, -0.5f,  color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,

        //FACE HAUT (y= 0.5)
        -0.5f,  0.5f, -0.5f,  color.r * 0.9f, color.g * 0.9f, color.b * 0.9f,
        0.5f,  0.5f, -0.5f,  color.r * 0.9f, color.g * 0.9f, color.b * 0.9f,
        0.5f,  0.5f,  0.5f,  color.r * 0.9f, color.g * 0.9f, color.b * 0.9f,
        0.5f,  0.5f,  0.5f,  color.r * 0.9f, color.g * 0.9f, color.b * 0.9f,
        -0.5f,  0.5f,  0.5f,  color.r * 0.9f, color.g * 0.9f, color.b * 0.9f,
        -0.5f,  0.5f, -0.5f,  color.r * 0.9f, color.g * 0.9f, color.b * 0.9f
    };

    //Création des buffers

    //générer le VAO et le VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //lier le VAO
    glBindVertexArray(VAO);

    //lier et remplir le VBO avec les sommets
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //config les attributs de sommets

    //attr0: position (3floats: x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // attr1: couleur (3floats: r, g, b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //délier
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


