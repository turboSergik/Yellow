cmake_minimum_required(VERSION 3.15)

project(WG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGET)

set(SOURCES
        main.cpp
        Camera.cpp
        Behaviour.cpp
        Point.cpp
        Line.cpp
        Post.cpp
        Train.cpp
        Database.cpp
        graphdrawing.cpp
        Transformable.cpp
        Component.cpp
        GameObject.cpp)

add_executable(WG ${SOURCES})

target_link_libraries(WG ${CONAN_LIBS})