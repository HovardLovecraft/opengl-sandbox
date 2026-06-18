//
//  main.cpp
//  
//
//  Created by Ivan Kuznetsov on 09.06.2026.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(GLFWwindow *window, float& mixFactor);


int main() {
    // --- Ініціалізація (те саме що було) ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Sandbox", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    // --- Шейдерна програма ---
    Shader ourShader(SHADER_DIR "/shader.vs", SHADER_DIR "/shader.fs");
    // --- Геометрія ---
    unsigned int indices[] = {
        0, 1, 2,  // First Triangle
        0, 2, 3   // Second Triangle
    };

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 2.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
    };

    unsigned int EBO, VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);                  // починаємо "записувати" в VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position VAO attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);                    // закінчили "записувати"


    // load and create a texture 1
    // -------------------------
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/textures/image.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        std::cout << width << " " << height << " this is the image data " << " \n";
        std::cout << nrChannels << " this is the image data " << " \n";
        glTexImage2D(
            GL_TEXTURE_2D,     // target: куди upload-имо
            0,                 // mipmap level: базовий рівень
            GL_RGB,            // internal format: як GPU зберігає текстуру
            width, height,     // розмір
            0,                 // border: історично має бути 0
            GL_RGB,            // input format: як data організована на CPU
            GL_UNSIGNED_BYTE,  // тип одного channel
            data               // pointer на pixels
        );
        
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {   
        std::cout << "Failed to load texture" << std::endl << "\n";
        std::cout << stbi_failure_reason() << "\n";
    }
    stbi_image_free(data);

    // load and create a texture 2
    // -------------------------
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // create 16x16 procedural texture
    int ptWidth, ptHeight, ptChannels, ptCellSize;
    unsigned int r, g, b;
    ptWidth = 16;
    ptHeight = 16;
    ptChannels = 3;
    ptCellSize = 4;
    std::vector<unsigned char> pixels(ptWidth * ptHeight * ptChannels);


    for (int row{0}; row < ptHeight; row++){
        for (int col{0}; col < ptWidth; col++){
            int checker = (row / ptCellSize + col / ptCellSize) % 2;
            if (checker == 0) {
                r = 255;
                g = 230;
                b = 40;
                int index = (row * ptWidth + col) * ptChannels;
                pixels[index + 0] = r;
                pixels[index + 1] = g;
                pixels[index + 2] = b;
            } else {
                r = 30;
                g = 80;
                b = 20;
                int index = (row * ptWidth + col) * ptChannels;
                pixels[index + 0] = r;
                pixels[index + 1] = g;
                pixels[index + 2] = b;
            }   
        }
    }
            glTexImage2D(
            GL_TEXTURE_2D,     // target: куди upload-имо
            0,                 // mipmap level: базовий рівень
            GL_RGB,            // internal format: як GPU зберігає текстуру
            ptWidth, ptHeight, // розмір
            0,                 // border: історично має бути 0
            GL_RGB,            // input format: як data організована на CPU
            GL_UNSIGNED_BYTE,  // тип одного channel
            pixels.data()               // pointer на pixels
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    
    float mixFactor = 0.5f;
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // --- Render loop ---
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window, mixFactor);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the texture

        ourShader.setFloat("mixFactor", mixFactor);
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- Cleanup ---
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, float& mixFactor) {
    double step = 0.005f;

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        mixFactor += step;
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        mixFactor -= step;
    }

    mixFactor = std::clamp(mixFactor, 0.0f, 1.0f);;
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}