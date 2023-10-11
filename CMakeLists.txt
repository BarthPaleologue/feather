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
        core/physics/*.cpp core/physics/*.h
)
add_executable(featherGL ${SRCS}
        core/physics/StretchConstraint.h
        core/physics/BendConstraint.h)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3")
target_link_libraries(${PROJECT_NAME} glfw)

find_package(glm REQUIRED)
message(STATUS "GLM included")

target_sources(${PROJECT_NAME} PRIVATE glad/src/glad.c)
target_include_directories(${PROJECT_NAME} PRIVATE glad/include/)
