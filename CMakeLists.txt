cmake_minimum_required(VERSION 3.15)

project(WG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
        Database.cpp
        static/graphdrawing.cpp
        core/Transform.cpp
        core/Component.cpp
        core/GameObject.cpp
        static/PrefabCreator.cpp
        core-components/renderers/LineRenderer.cpp
        core-components/renderers/Renderer.cpp
        core-components/renderers/SpriteRenderer.cpp
        core-components/renderers/CircleRenderer.cpp)

add_executable(WG ${SOURCES})

target_link_libraries(WG ${CONAN_LIBS})
