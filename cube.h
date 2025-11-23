#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class Cube {
public:
    GLuint VAO, VBO;

    Cube(glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.2f));

    ~Cube();

    void draw();

private:
    void createCube(glm::vec3 color);

};

#endif
