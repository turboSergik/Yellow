cmake_minimum_required(VERSION 3.5)

project(WGGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER "gcc")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER "Visual Studio")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(COMPILER "apple-clang")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(COMPILER "clang")
endif()

execute_process(COMMAND conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan)
execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR} --build=missing --install-folder=${CMAKE_BINARY_DIR} -s build_type=${CMAKE_BUILD_TYPE} -s compiler=${COMPILER})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGET)

set(SOURCES 
    main.cpp
    graphdrawing.cpp)

add_executable(WGGame ${SOURCES})

target_link_libraries(WGGame ${CONAN_LIBS})

