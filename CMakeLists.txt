cmake_minimum_required(VERSION 3.25)
project(Power4)

set(CMAKE_CXX_STANDARD 23)

add_executable(Power4
        src/main.cpp
        src/game/Power4Game.hpp
        src/game/Game.hpp
        src/util/Coord.hpp
)

target_include_directories(Power4 PRIVATE thirdparty/include)
