cmake_minimum_required(VERSION 3.25)
project(Power4)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
        cpptrace
        GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
        GIT_TAG v0.1
)
FetchContent_MakeAvailable(cpptrace)

add_executable(Power4
        src/main.cpp
        src/game/Power4Game.hpp
        src/game/Game.hpp
        src/util/Coord.hpp
        src/util/MathUtils.hpp
        src/util/OutOfRangeException.hpp
        src/util/TracedException.hpp
)

target_compile_options(
        Power4

        PRIVATE
        -pedantic
        -pedantic-errors
        -Wall
        -Wextra
)

target_include_directories(Power4 SYSTEM PRIVATE thirdparty/include)

target_link_libraries(Power4 cpptrace)

#set(CMAKE_BUILD_TYPE RelWithDebInfo) # uncomment to enable debug symbols, but messes with CLion's debugger