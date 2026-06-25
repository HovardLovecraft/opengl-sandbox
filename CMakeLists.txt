cmake_minimum_required(VERSION 3.20)
project(opengl-sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GLFW
set(GLFW_DIR /opt/homebrew/opt/glfw)
# GLEW
set(GLEW_DIR /opt/homebrew/opt/glew)
#GLM
set(GLM_DIR /opt/homebrew/opt/glm)

add_executable(sandbox
    src/main.cpp
    src/OpenGLDebug.cpp
)

target_include_directories(sandbox PRIVATE
    ${GLFW_DIR}/include
    ${GLEW_DIR}/include
    ${GLM_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(sandbox PRIVATE
    SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders"
)

target_link_libraries(sandbox PRIVATE
    ${GLFW_DIR}/lib/libglfw.dylib
    ${GLEW_DIR}/lib/libGLEW.dylib
    "-framework OpenGL"
)
