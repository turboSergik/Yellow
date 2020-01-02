cmake_minimum_required(VERSION 3.13)

project(WG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/conancommand.cmake)
set_conan_params()

execute_process(COMMAND conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan)
if (WIN32)
    execute_process(COMMAND ${CMAKE_COMMAND} -E echo ${CONAN_INSTALL_COMMAND} COMMAND cmd)
elseif(UNIX)
    execute_process(COMMAND ${CMAKE_COMMAND} -E echo ${CONAN_INSTALL_COMMAND} COMMAND sh)
endif()

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
        static/Prefabs.cpp
        core-components/renderers/LineRenderer.cpp
        core-components/renderers/Renderer.cpp
        core-components/renderers/SpriteRenderer.cpp
        core-components/renderers/CircleRenderer.cpp
        custom-components/CameraController.cpp
        static/Time.cpp
        network/Connection.cpp
        network/Packet.cpp
        network/PacketQueue.cpp
        custom-components/GraphController.cpp
        utility/GraphVisualizer.cpp
        static/Input.cpp
        static/MethodsPool.cpp
        utility/methodWrapper.cpp
        network/Network.cpp
        utility/Event.cpp
        custom-components/PlayerController.cpp
        core-components/RigidBody.cpp
        linalg/Vector2.cpp)

add_executable(WG ${SOURCES})

target_link_libraries(WG ${CONAN_LIBS})
