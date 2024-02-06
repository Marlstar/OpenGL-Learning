#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <SHADERCLASS/shader.h>

// Core functionalities
#include <CUSTOM/GLmanager.hpp>
#include <CUSTOM/window.hpp>

#include <CUSTOM/renderer.hpp>
#include <CUSTOM/textureManager.hpp>
#include <CUSTOM/inputHandler.hpp>
#include <CUSTOM/tilemap.hpp>

// External libraries
#define STB_IMAGE_IMPLEMENTATION
#include <OTHER/stb_image.h>
#include <OTHER/termcolor.hpp>

// Libraries
#include <iostream>
#include <string>
#include <array>
#include <vector>

// Important settings for various things
struct
{
    struct windowSettings_struct {
        int width = 800;
        int height = 600;
        std::string title = "the pinnacle of epicness";
    } window;

    struct renderSettings_struct {
        float clearColour[4] = { 0.53f, 0.81f, 0.92f, 1.0f }; // RGBA, scaled from 0-1 rather than 0-255
        bool pixelArt = true;
    } rendering;
} settings;

// Resize the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


// GL core stuff
GLmanager glManager;

// Main window
Window window;

// Rendering
Renderer renderer;

// Texture stuff
TextureManager textureManager;

// Tilemap
TilemapManager tilemapManager;


int main()
{
    std::cout << colour::yellow << "Warning: app probably has >87346 memory leaks, enjoy =)" << colour::reset << std::endl;

    glManager.initGL();

    
    // Creating the window
    window.createWindow(settings.window.width, settings.window.height, settings.window.title);
    glfwMakeContextCurrent(window.window);
    
    glManager.checkGlad();
    
    glViewport(0, 0, settings.window.width, settings.window.height);
    glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);

    // #=====================# \\ 
    // Tilemap and world stuff \\ 
    // #=====================# \\ 
    

    // #============================# \\ 
    // Vertices & Vertex Shader Stuff \\ 
    // #============================# \\ 
    
    float vertices[] = {
    // Position           // Colour             // Texture coords
    -0.9f, 0.9f, 0.0f,    1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
    -0.9f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
    0.9f, 0.9f, 0.0f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
    
    0.9f, 0.9f, 0.0f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f,
    -0.9f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
    0.9f, -0.9f, 0.0f,    1.0f, 0.0f, 1.0f,     1.0f, 0.0f
    };
    

    // Vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    

    textureManager.initTexture("Dirt", "resources/textures/dirt.png");
    textureManager.initTexture("Grass", "resources/textures/grass.png");

    //tilemapManager.initTilemap()

    // OLD | loadTexture("resources/textures/blueuniverse.png");

    Shader basicShader("resources/shaders/VertexShader.glsl", "resources/shaders/FragmentShader.glsl");

    // VAO (Vertex Array Object)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Bind to the buffer containi6ng the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    int attributeCount = 2;
    int count;

    // Position:        ID: 0, Size: 3
    // Colour:          ID: 1, Size: 3
    // Texture coords:  ID: 2, Size: 2
    int attributes[] = {
        3, 3, 2
    };

    int stride = 0;
    for (int attrib : attributes) { stride += attrib; }

    int offset = 0;
    for (int i = 0; i < (sizeof(attributes) / sizeof(int)); i++)
    {
        count = attributes[i];

        glVertexAttribPointer(i, count, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(sizeof(float) * offset));
        glEnableVertexAttribArray(i);

        offset += count;
    }
    
    glfwSetKeyCallback(window.window, keyCallback);
    
    // #=================# \\ 
    // Main Rendering Loop \\ 
    // #=================# \\ 
    while (!glfwWindowShouldClose(window.window))
    {
        // Input \\ 
        bool spacePressed = checkKey(GLFW_KEY_SPACE);

        // Rendering \\ 
        // Setting the clear colour
        glClearColor(settings.rendering.clearColour[0], settings.rendering.clearColour[1], settings.rendering.clearColour[2], settings.rendering.clearColour[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        // Set the texture to use
        if (spacePressed)
        {
            textureManager.useTexture((char*)"Grass");
        }
        else
        {
            textureManager.useTexture((char*)"Dirt");
        }

        // Drawing the triangles =)
        int numTriangles;
        numTriangles = sizeof(vertices) / sizeof(float) / 8;
        //glUseProgram(shaderProgram);
        basicShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, numTriangles);

           
        // Check and call the events and swap the buffers
        // Can kinda guess what this does but idk
        // Probably swaps the currently shown frame buffer for the newly rendered one
        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    settings.window.width = width;
    settings.window.height = height;
}



