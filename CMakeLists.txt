cmake_minimum_required(VERSION 3.5)

project(WGGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGET)

set(SOURCES 
    main.cpp
    graphdrawing.cpp)

add_executable(WGGame ${SOURCES})

target_link_libraries(WGGame ${CONAN_LIBS})

