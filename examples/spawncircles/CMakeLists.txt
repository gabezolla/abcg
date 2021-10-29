cmake_minimum_required(VERSION 3.19)

project(spawncircles)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp)
enable_abcg(${PROJECT_NAME})