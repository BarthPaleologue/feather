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
        core/shadows/*.cpp core/shadows/*.h
        imgui/*.cpp imgui/*.h
)


add_executable(featherGL ${SRCS} ${IMGUI_SRC}
        core/physics/UniformAccelerationField.h
        core/physics/DihedralBendConstraint.h
        core/physics/ConstraintHelper.h
        core/Renderable.h
        core/AABB.h
        core/AABBHelper.h
        core/physics/GeneralizedVolumeConstraint.h
        core/PickResult.h
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3")

find_package(glm REQUIRED)
message(STATUS "GLM included")

target_link_libraries(${PROJECT_NAME} glfw glm::glm)

target_sources(${PROJECT_NAME} PRIVATE glad/src/glad.c)
target_include_directories(${PROJECT_NAME} PRIVATE glad/include/)

target_sources(${PROJECT_NAME} PRIVATE Eigen/Dense)
target_include_directories(${PROJECT_NAME} PRIVATE Eigen/)

# Set optimization level to -O3
target_compile_options(${PROJECT_NAME} PRIVATE -O3)
