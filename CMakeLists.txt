cmake_minimum_required(VERSION 3.21)
project(featherGL)

set(CMAKE_CXX_STANDARD 17)

include_directories(materials core utils drawables)
file(GLOB SRCS ./*.h ./*.cpp
        ./core/*.cpp ./core/*.h
        ./core/cameras/*.cpp ./core/cameras/*.h
        ./drawables/*.cpp ./drawables/*.h
        ./materials/*.cpp ./materials/*.h
        ./utils/*.cpp ./utils/*.h
        )
add_executable(featherGL ${SRCS})

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(glm)
target_link_libraries(${PROJECT_NAME} glm)

target_sources(${PROJECT_NAME} PRIVATE glad/src/glad.c)
target_include_directories(${PROJECT_NAME} PRIVATE glad/include/)
