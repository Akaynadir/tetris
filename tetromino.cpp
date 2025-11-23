#include "tetromino.h"

Tetromino::Tetromino() {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(NULL)));
        seeded = true;
    }

    type = static_cast<TetrominoType>(rand() % 7);
    rotation = 0;

    //position de départ
    x = 4;
    y = 10;

    initializeShape();
}

Tetromino::Tetromino(TetrominoType t, int startX, int startY) : type(t), x(startX), y(startY), rotation(0) {
    initializeShape();
}

std::vector<glm::ivec2> Tetromino::getAbsolutePositions() const {
    std::vector<glm::ivec2> positions;
    for (const auto& block : blocks) {
        positions.push_back(glm::ivec2(x + block.x, y + block.y));
    }
    return positions;
}

void Tetromino::move(int dx, int dy) {
    x += dx;
    y += dy;
}


void Tetromino::rotate() {
    if (type == O_PIECE) return;

    rotation = (rotation + 1) % 4;

    std::vector<glm::ivec2> newBlocks;
    for (const auto& block : blocks) {
        //rotation 90° horaire: (x, y) -> (y, -x)
        newBlocks.push_back(glm::ivec2(block.y, -block.x));
    }
    blocks = newBlocks;

    //Normaliser
    normalizeBlocks();
}

void Tetromino::initializeShape() {
    blocks.clear();

    switch (type) {
    case I_PIECE:
        blocks.push_back(glm::ivec2(0, 0));
        blocks.push_back(glm::ivec2(1, 0));
        blocks.push_back(glm::ivec2(2, 0));
        blocks.push_back(glm::ivec2(3, 0));
        color = glm::vec3(1.0f, 1.0f, 0.0f);
        break;

    case O_PIECE:
        blocks.push_back(glm::ivec2(0, 0));
        blocks.push_back(glm::ivec2(1, 0));
        blocks.push_back(glm::ivec2(0, 1));
        blocks.push_back(glm::ivec2(1, 1));
        color = glm::vec3(0.0f, 1.0f, 1.0f);
        break;

    case T_PIECE:
        blocks.push_back(glm::ivec2(1, 1));
        blocks.push_back(glm::ivec2(0, 0));
        blocks.push_back(glm::ivec2(1, 0));
        blocks.push_back(glm::ivec2(2, 0));
        color = glm::vec3(1.0f, 0.0f, 0.0f);
        break;

    case S_PIECE:
        blocks.push_back(glm::ivec2(1, 1));
        blocks.push_back(glm::ivec2(2, 1));
        blocks.push_back(glm::ivec2(0, 0));
        blocks.push_back(glm::ivec2(1, 0));
        color = glm::vec3(0.0f, 1.0f, 0.0f);
        break;

    case Z_PIECE:
        blocks.push_back(glm::ivec2(0, 1));
        blocks.push_back(glm::ivec2(1, 1));
        blocks.push_back(glm::ivec2(1, 0));
        blocks.push_back(glm::ivec2(2, 0));
        color = glm::vec3(0.5f, 0.0f, 0.5f);
        break;

    case J_PIECE:
        blocks.push_back(glm::ivec2(0, 1));
        blocks.push_back(glm::ivec2(0, 0));
        blocks.push_back(glm::ivec2(1, 0));
        blocks.push_back(glm::ivec2(2, 0));
        color = glm::vec3(0.0f, 0.0f, 1.0f);
        break;

    case L_PIECE:
        blocks.push_back(glm::ivec2(2, 1));
        blocks.push_back(glm::ivec2(0, 0));
        blocks.push_back(glm::ivec2(1, 0));
        blocks.push_back(glm::ivec2(2, 0));
        color = glm::vec3(1.0f, 0.5f, 0.0f);
        break;
    }
}

void Tetromino::normalizeBlocks() {
    if (blocks.empty()) return;

    //min x et y
    int minX = blocks[0].x;
    int minY = blocks[0].y;

    for (const auto& block : blocks) {
        if (block.x < minX) minX = block.x;
        if (block.y < minY) minY = block.y;
    }

    for (auto& block : blocks) {
        block.x -= minX;
        block.y -= minY;
    }
}





