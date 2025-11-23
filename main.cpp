#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders.h"
#include "cube.h"
#include "terrain.h"
#include "tetromino.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int TERRAIN_WIDTH = 10;
const int TERRAIN_HEIGHT = 12;

//variables pour la caméra
float cameraX = 5.0f;
float cameraY = 8.0f;
float cameraZ = 25.0f;
bool wireframeMode = false;

//variables pour le jeu
Tetromino* currentPiece = nullptr;
Terrain* terrain = nullptr;
double lastFallTime = 0.0;
const double chute = 1.0; //en seconde
bool finDuJeu = false;
int score = 0;
int lignesSupp = 0;






//vérifier si la pièce peut se déplacer à une position donnée
bool canMoveTo(int newX, int newY, Tetromino* piece) {
    if (!piece || !terrain) return false;

    int oldX= piece->x;
    int oldY = piece->y;
    piece->x=newX;
    piece->y= newY;

    auto positions = piece->getAbsolutePositions();
    for (const auto& pos:positions) {
        //avec le terrain
        if (pos.x<0 || pos.x>=TERRAIN_WIDTH || pos.y<0){
            piece->x= oldX;
            piece->y= oldY;
            return false;
        }
        //avec les cubes posés
        if (pos.y<TERRAIN_HEIGHT && !terrain->isFree(pos.x, pos.y)){
            piece->x= oldX;
            piece->y= oldY;
            return false;
        }
    }
    piece->x=oldX;
    piece->y=oldY;
    return true;
}

//vérifier si la pièce peut tourner
bool canRotate(Tetromino* piece) {
    if (!piece || !terrain) return false;

    int oldRotation= piece->rotation;
    auto oldBlocks= piece->blocks;
    //tester la rotation
    piece->rotate();
    auto positions= piece->getAbsolutePositions();

    //vérifier chaque cube
    for (const auto& pos : positions) {
        if (pos.x<0 || pos.x>=TERRAIN_WIDTH || pos.y< 0 ||
            (pos.y <TERRAIN_HEIGHT && !terrain->isFree(pos.x, pos.y))) {
            piece->rotation = oldRotation;
            piece->blocks = oldBlocks;
            return false;
        }
    }
    piece->rotation= oldRotation;
    piece->blocks= oldBlocks;
    return true;
}

//verrouiller la pièce sur le terrain
void lockPiece() {
    if (!currentPiece || !terrain) return;
    auto positions = currentPiece->getAbsolutePositions();
    //placer chaque cube de la pièce sur le terrain
    for (const auto& pos : positions) {
        if (pos.y>=0 && pos.y<TERRAIN_HEIGHT) {
            terrain->placeCube(pos.x, pos.y, currentPiece->color);
        }
    }

    std::cout<<"lock ok";

    //supression des lignes complètes
    int linesCleared = terrain->clearCompleteLines();

    //Maj du score le score
    if (linesCleared > 0) {
        lignesSupp += linesCleared;

        //système de score
        //1 ligne = 100 pts, 2 = 300, 3 = 500, 4 = 800 pts
        int points[] = { 0, 100, 300, 500, 800 };
        if (linesCleared <= 4) {
            score += points[linesCleared];
        }
        else {
            score += 800 + (linesCleared - 4) * 200;
        }

        std::cout<<"SCORE: "<<score<<", LIGNES: "<<lignesSupp;
    }
}

//création d'une nouvelle pièce
void spawnNewPiece() {
    delete currentPiece;
    currentPiece = new Tetromino();
    if (!canMoveTo(currentPiece->x, currentPiece->y, currentPiece)) {
        std::cout<<"\nFin du jeux";
        finDuJeu = true;
    }
}

//faire tomber la pièce
void updateGame(double currentTime) {
    if (finDuJeu || !currentPiece) return;
    if (currentTime - lastFallTime >= chute) {
        lastFallTime = currentTime;
        if (canMoveTo(currentPiece->x, currentPiece->y - 1, currentPiece)) {
            currentPiece->move(0, -1);
            std::cout << "Pièce descendue automatiquement à y=" << currentPiece->y;
        }
        else {
            lockPiece();
            spawnNewPiece();
        }
    }
}


//les callbacks
//fonc. appelée lors d'une erreur GLFW
void error_callback(int error, const char* description) {
    std::cerr << "Erreur GLFW " << error << ": " << description;
}
//fonc. appelée quand une touche est pressée
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //esc pour quitter
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    //si le jeux est fini, juste esc et R fonctionnent
    if (finDuJeu) {
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            //recommencer le jeu
            terrain->grid.assign(TERRAIN_WIDTH, std::vector<bool>(TERRAIN_HEIGHT, false));
            terrain->placedCubes.clear();
            finDuJeu = false;
            score = 0;
            lignesSupp = 0;
            spawnNewPiece();
            std::cout<<"\nNouveau Jeux";
        }
        return;
    }

    //controle de la pièce
    if (currentPiece && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        //A ou flèche GAUCHE : déplacer à gauche
        if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
            if (canMoveTo(currentPiece->x - 1, currentPiece->y, currentPiece)) {
                currentPiece->move(-1, 0);
            }
        }

        //E ou D ou flèche DROITE : déplacer à droite
        if (key == GLFW_KEY_E || key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
            if (canMoveTo(currentPiece->x + 1, currentPiece->y, currentPiece)) {
                currentPiece->move(1, 0);
            }
        }

        //S ou flèche BAS : descendre plus vite
        if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
            if (canMoveTo(currentPiece->x, currentPiece->y - 1, currentPiece)) {
                currentPiece->move(0, -1);
            }
            else {
                // Si ne peut pas descendre, verrouiller immédiatement
                lockPiece();
                spawnNewPiece();
            }
        }

        // ESPACE ou flèche HAUT : rotation
        if (key == GLFW_KEY_SPACE || key == GLFW_KEY_UP) {
            if (canRotate(currentPiece)) {
                currentPiece->rotate();
            }
        }
    }

    //Débugging
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        float speed = 1.0f;

        if (key == GLFW_KEY_PAGE_UP) cameraY += speed;
        if (key == GLFW_KEY_PAGE_DOWN) cameraY -= speed;

        // W pour wireframe mode
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            wireframeMode = !wireframeMode;
            if (wireframeMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                std::cout<<"Mode wireframe activé";
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                std::cout<<"Mode normal activé";
            }
        }

        // C pour afficher la position de la caméra
        if (key == GLFW_KEY_C) {
            std::cout << "Caméra: (" << cameraX << ", " << cameraY << ", " << cameraZ << ")";
        }
    }
}



int main() {
    //glfw
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Échec de l'initialisation de GLFW";
        return -1;
    }



    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //création de la fenêtre
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "Tetris 3D - Phase 4", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    //glew
    glewExperimental=GL_TRUE;
    GLenum err=glewInit();
    if(err!=GLEW_OK){
        std::cerr<<glewGetErrorString(err);
        glfwTerminate();
        return -1;
    }



    //config opengl
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    //création des shaders
    GLuint shaderProgram = createShaderProgram();
    if (shaderProgram == 0) {
        glfwTerminate();
        return -1;
    }



    //config des matrices
    glm::mat4 projection=glm::perspective(
        glm::radians(45.0f),
        (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,
        0.1f,
        100.0f
        );

    glm::mat4 model=glm::mat4(1.0f);


    //création du terrain
    std::cout<<"Création du terrain de jeu.";
    terrain= new Terrain(TERRAIN_WIDTH, TERRAIN_HEIGHT);

    //le cube du rendu
    Cube renderCube(glm::vec3(1.0f, 0.5f, 0.2f));

    //1ere pièce
    std::cout<<"Génération de la première pièce";
    spawnNewPiece();
    lastFallTime= glfwGetTime();



    std::cout<<"TETRIS 3D - JEU LANCÉ:";
    std::cout<<"Terrain: "<< TERRAIN_WIDTH << "x" << TERRAIN_HEIGHT;
    std::cout<<"Vitesse de chute: "<<chute<<" seconde(s)";
    std::cout<<"\nSYSTÈME DE SCORE";
    std::cout<<"1 ligne  = 100 points";
    std::cout<<"2 lignes = 300 points";
    std::cout<<"3 lignes = 500 points";
    std::cout<<"4 lignes = 800 points";
    std::cout<<"\nCONTRÔLES";
    std::cout<<"A / ← : Gauche";
    std::cout<<"E / D / → : Droite";
    std::cout<<"S / ↓  : Descendre";
    std::cout<<"ESPACE / ↑ : Rotation";
    std::cout<<"R      : Recommencer (après game over)";
    std::cout<<"ESC    : Quitter";
    std::cout<<"W      : Wireframe (débogage)";




    //Boucle du rendu
    while (!glfwWindowShouldClose(window)) {
        //Màj le jeu (chute automatique)
        double currentTime = glfwGetTime();
        updateGame(currentTime);

        //effacer l'écran
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Utilisation du programme shader
        glUseProgram(shaderProgram);

        //Màj la caméra
        glm::mat4 view=glm::lookAt(
            glm::vec3(cameraX, cameraY, cameraZ),
            glm::vec3(5.0f, 6.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        //Envoyer les matrices
        GLuint modelLoc=glGetUniformLocation(shaderProgram, "model");
        GLuint viewLoc=glGetUniformLocation(shaderProgram, "view");
        GLuint projectionLoc=glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc,1,GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc,1, GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1,GL_FALSE,glm::value_ptr(projection));

        //Dessiner le terrain
        terrain->draw(renderCube, shaderProgram);

        //Dessiner la pièce active
        if (currentPiece && !finDuJeu) {
            terrain->drawTetromino(*currentPiece, renderCube, shaderProgram);
        }
        //Échanger les buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Nettoyage
    std::cout<<"Fermeture du programme";
    delete currentPiece;
    delete terrain;
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
