cmake_minimum_required(VERSION 2.6)
project(Ex1Aula)
add_subdirectory(lib)
add_executable(ex1 main.c)
target_link_libraries(ex1 aritimetica)

