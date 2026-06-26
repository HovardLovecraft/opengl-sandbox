//
//  main.cpp
//  
//
//  Created by Ivan Kuznetsov on 09.06.2026.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

#include "Camera.h"
#include "OpenGLDebug.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(GLFWwindow *window, float& mixFactor);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float lastX =  800.0f / 2.0;
float lastY =  600.0f / 2.0;


//time
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

//Lighting
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightPos(0.0f, 0.0f, 1.0f);
glm::vec3 lampColor(1.0f, 0.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);




int main() {
    // --- Ініціалізація (те саме що було) ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    #ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Sandbox", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewError) << "\n";
        return -1;
    }

    #ifndef NDEBUG
    glGetError(); // GLEW may leave a benign GL_INVALID_ENUM in core profile.
    setupOpenGLDebugCallback();
    #endif

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;

    // callback register section
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // --- Шейдерна програма ---
    Shader objShader(SHADER_DIR "/shader.vs", SHADER_DIR "/shader.fs");
    Shader lampShader(SHADER_DIR "/shader.vs", SHADER_DIR "/lamp.fs");
    // --- Геометрія ---
    unsigned int indices[] = {
        0, 1, 2,  // First Triangle
        0, 2, 3   // Second Triangle
    };

    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int EBO, cubeVAO, VBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(cubeVAO);                  // починаємо "записувати" в VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position VAO attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    //light VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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

    // --- Render loop ---
    while (!glfwWindowShouldClose(window)) {
        //timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame; 
        
        // input
        // -----
        processInput(window, mixFactor);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        //Perspective
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        //View
        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = camera.GetViewMatrix();

        objShader.use();
        objShader.setInt("texture1", 0);
        objShader.setInt("texture2", 1);
        objShader.setVec3("lightColor", lightColor);
        objShader.setFloat("mixFactor", mixFactor);

        int viewLoc = glGetUniformLocation(objShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(objShader.ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Cube loop
        glBindVertexArray(cubeVAO);
        for(size_t i = 0; i < 10; i++)
        {   
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle; 
            i % 3 == 0 ? angle = 20.0f * i : angle = (float)glfwGetTime() * 15.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            objShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lampShader.use();
        lampShader.setVec3("lightColor", lampColor);

        int lampViewLoc = glGetUniformLocation(lampShader.ID, "view");
        glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int lampProjLoc = glGetUniformLocation(lampShader.ID, "projection");
        glUniformMatrix4fv(lampProjLoc, 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(lightVAO);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lampShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- Cleanup ---
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, float& mixFactor) {
    double step = 0.005f;
    float cameraSpeed = static_cast<float>(2.5 * deltaTime);

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        mixFactor += step;
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        mixFactor -= step;
    }

    mixFactor = std::clamp(mixFactor, 0.0f, 1.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
        
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the mouse moves, this callback is called
// mouse position -> offset -> sensitivity -> yaw/pitch -> clamp pitch -> cameraFront
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn); // casting double to float
    float ypos = static_cast<float>(yposIn);

    // checking if mouse is located in first position
    // so that the first offset is not a huge jump from the center of the window to the actual mouse position.
    if (firstMouse) 
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX; // the difference between the current and previous position.
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos; // current position becomes last position
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
