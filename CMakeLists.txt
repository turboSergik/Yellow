cmake_minimum_required(VERSION 3.13)

project(WG LANGUAGES CXX)

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
        core-components/Camera.cpp
        custom-components/Behaviour.cpp
        custom-components/Point.cpp
        custom-components/Line.cpp
        custom-components/Post.cpp
        custom-components/Train.cpp
        static/Database.cpp
        core/Transform.cpp
        core/Component.cpp
        core/GameObject.cpp
        static/PrefabCreator.cpp
        core-components/renderers/LineRenderer.cpp
        core-components/renderers/Renderer.cpp
        core-components/renderers/SpriteRenderer.cpp
        core-components/renderers/CircleRenderer.cpp
        custom-components/CameraController.cpp
        static/Time.cpp
        Networking/Connection.cpp
        Networking/Packet.cpp
        Networking/PacketQueue.cpp
        custom-components/GraphController.cpp
        utility/GraphVisualizer.cpp
        static/Input.cpp
        static/MethodsPool.cpp
        utility/methodWrapper.cpp)

add_executable(WG ${SOURCES})

target_link_libraries(WG ${CONAN_LIBS})
