cmake_minimum_required(VERSION 3.19)

project(elite-aim)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp)
enable_abcg(${PROJECT_NAME})