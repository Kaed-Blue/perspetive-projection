cmake_minimum_required(VERSION 3.24)
project(perspectiveProjection)

set(SOURCE_FILES cpp/main.cpp cpp/Engine.cpp cpp/Mesh.cpp cpp/Renderer.cpp cpp/Camera.cpp cpp/Ilumination.cpp cpp/VecMath.cpp)

add_executable(main ${SOURCE_FILES})

find_package(SDL3 REQUIRED)
target_link_libraries(main PRIVATE SDL3::SDL3)
target_include_directories(main PRIVATE h)