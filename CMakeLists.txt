cmake_minimum_required(VERSION 3.5)

project(tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tetris main.cpp
    cube.h cube.cpp
    terrain.h terrain.cpp
    shaders.h
    tetromino.h tetromino.cpp

)
target_link_libraries(tetris glfw GLEW libGLEW.so libGLU.so libGL.so)

include(GNUInstallDirs)
install(TARGETS tetris
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
