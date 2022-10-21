cmake_minimum_required(VERSION 3.21)
project(featherGL)

set(CMAKE_CXX_STANDARD 17)

include_directories(core/materials core core/utils core/meshes core/lights core/cameras)
file(GLOB SRCS ./*.h ./*.cpp
        ./core/*.cpp ./core/*.h
        ./core/cameras/*.cpp ./core/cameras/*.h
        ./core/lights/*.cpp ./core/lights/*.h
        core/meshes/*.cpp core/meshes/*.h
        core/materials/*.cpp core/materials/*.h
        core/utils/*.cpp core/utils/*.h
        )
add_executable(featherGL ${SRCS})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(glm)
target_link_libraries(${PROJECT_NAME} glm)

target_sources(${PROJECT_NAME} PRIVATE glad/src/glad.c)
target_include_directories(${PROJECT_NAME} PRIVATE glad/include/)
