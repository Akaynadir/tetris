#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <algorithm>
#include "cube.h"
#include <iostream>

class Tetromino;

class Terrain {
public:
    int width;   //LArgeur
    int height;  //Hauteur

    std::vector<std::vector<bool>> grid;

    struct CubeData {
        int x, y;
        glm::vec3 color;
    };
    std::vector<CubeData> placedCubes;

    Terrain(int w, int h);

    //les bords
    void drawBorders(Cube& cube, GLuint shaderProgram);
    //la gille au sol
    void drawGrid(Cube& cube, GLuint shaderProgram);
    //les cubes posés
    void drawPlacedCubes(Cube& cube, GLuint shaderProgram);

    bool isFree(int x, int y) {
        if (x<0 || x>=width || y<0 || y>=height) {
            return false;
        }
        return !grid[x][y];
    }

    //Placer un cube dans le terrain
    void placeCube(int x, int y, glm::vec3 color) {
        if (x>= 0 && x< width && y >= 0&& y < height) {
            grid[x][y] = true;
            placedCubes.push_back({ x, y, color });
        }
    }

    //Effacer un cube du terrain
    void removeCube(int x, int y) {
        if (x>= 0 && x< width && y >= 0&& y < height) {
            grid[x][y] = false;

            placedCubes.erase(
                std::remove_if(placedCubes.begin(), placedCubes.end(),
                               [x, y](const CubeData& c) { return c.x == x && c.y == y; }),
                placedCubes.end()
                );
        }
    }

    bool isLineComplete(int y) {
        if (y<0 || y >=height) return false;

        for (int x=0; x<width; x++) {
            if (!grid[x][y]) {
                return false;
            }
        }
        return true;
    }

    //supprimer une ligne complète
    void clearLine(int y) {
        if (y < 0||y >= height) return;

        //supression de tous les cubes de cette ligne
        for (int x = 0; x < width; x++) {
            grid[x][y] = false;
        }

        placedCubes.erase(
            std::remove_if(placedCubes.begin(), placedCubes.end(),
                           [y](const CubeData& c) {
                        return c.y == y;
            }),
            placedCubes.end()
            );
    }

    //faire descendre tous les cubes au-dessus d'une ligne
    void dropCubesAbove(int clearedLine) {
        for (int y=clearedLine; y<height-1;y++) {
            for (int x = 0; x < width; x++) {
                grid[x][y] = grid[x][y+1];
            }
        }

        for (int x=0; x<width; x++) {
            grid[x][height-1] = false;
        }

        for (auto& cube : placedCubes) {
            if (cube.y > clearedLine) {
                cube.y--;
            }
        }
    }

    int clearCompleteLines() {
        int linesCleared = 0;
        for (int y=0; y<height; y++) {
            if (isLineComplete(y)) {
                clearLine(y);
                dropCubesAbove(y);
                linesCleared++;
                y--;
            }
        }
        return linesCleared;
    }

    //dessiner le terrain
    void draw(Cube& cube, GLuint shaderProgram) {
        drawGrid(cube, shaderProgram);
        drawBorders(cube, shaderProgram);
        drawPlacedCubes(cube, shaderProgram);
    }
    void drawTetromino(const Tetromino& piece, Cube& cube, GLuint shaderProgram);
};

#include "tetromino.h"

inline void Terrain::drawTetromino(const Tetromino& piece, Cube& cube, GLuint shaderProgram) {
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    Cube pieceCube(piece.color);
    auto positions = piece.getAbsolutePositions();
    for (const auto& pos : positions) {
        glm::mat4 model=glm::mat4(1.0f);
        model=glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
        glUniformMatrix4fv(modelLoc,1, GL_FALSE, glm::value_ptr(model));
        pieceCube.draw();
    }
}

#endif
