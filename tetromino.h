#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>
#include <glm/glm.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

//types
enum TetrominoType {
    I_PIECE,
    O_PIECE,
    T_PIECE,
    S_PIECE,
    Z_PIECE,
    J_PIECE,
    L_PIECE
};

class Tetromino {
public:
    TetrominoType type;
    glm::vec3 color;
    int x, y;
    int rotation;

    std::vector<glm::ivec2> blocks;

    //Cons d'une pièce aléatoire
    Tetromino();
    //Cons avec type spécifique
    Tetromino(TetrominoType t, int startX, int startY);

    std::vector<glm::ivec2> getAbsolutePositions() const;

    void move(int dx, int dy);

    void rotate();

private:
    void initializeShape();
    void normalizeBlocks();
};

#endif
