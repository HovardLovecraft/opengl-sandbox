cmake_minimum_required(VERSION 3.20)
project(opengl-sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW
set(GLFW_DIR /opt/homebrew/opt/glfw)
# GLEW
set(GLEW_DIR /opt/homebrew/opt/glew)

add_executable(sandbox src/main.cpp)

target_include_directories(sandbox PRIVATE
    ${GLFW_DIR}/include
    ${GLEW_DIR}/include
)

target_link_libraries(sandbox PRIVATE
    ${GLFW_DIR}/lib/libglfw.dylib
    ${GLEW_DIR}/lib/libGLEW.dylib
    "-framework OpenGL"
)