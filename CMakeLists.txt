cmake_minimum_required(VERSION 3.13)
project(nim C)

set(CMAKE_C_STANDARD 99)

add_executable(sudoku main.c solver.c mainaux.c game.c parser.c)
