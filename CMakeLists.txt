cmake_minimum_required(VERSION 3.21)
project(featherGL)

set(CMAKE_CXX_STANDARD 17)

set(SRCS main.cpp utils.cpp Drawable.cpp Material.cpp Vector3.cpp DefaultMaterial.cpp Scene.cpp)

set(HEADERS Drawable.h Material.h Vector3.h DefaultMaterial.h Scene.h)

add_executable(featherGL ${SRCS} ${HEADERS})

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(glfw)
target_link_libraries(${PROJECT_NAME} glfw)

target_sources(${PROJECT_NAME} PRIVATE glad/src/glad.c)
target_include_directories(${PROJECT_NAME} PRIVATE glad/include/)
