cmake_minimum_required(VERSION 3.25)
project(Power4)

set(CMAKE_CXX_STANDARD 23)

add_executable(Power4 src/main.cpp src/game/Power4Board.cpp src/game/Power4Board.hpp)
