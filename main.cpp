#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <SHADERCLASS/shader.h>

// Core functionalities
#include <CUSTOM/textureManager.h>
#include <CUSTOM/inputHandler.h>
#include <CUSTOM/tilemap.h>

#define STB_IMAGE_IMPLEMENTATION
#include <OTHER/stb_image.h>

#include <iostream>
#include <string>
#include <array>
#include <vector>

#include <OTHER/termcolor.hpp>


// Window settings
struct windowSettings_struct{
    int width = 800;
    int height = 600;
    std::string title = "the pinnacle of epicness";    
} windowSettings;

// Rendering settings
struct renderSettings_struct{
    float clearColour[4] = { 0.53f, 0.81f, 0.92f, 1.0f }; // RGBA, scaled from 0-1 rather than 0-255
    bool pixelArt = true;
} renderSettings;

// Resize the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Texture stuff
// Texture loading function (moved to own file)
//void loadTexture(const char* texturePath, bool flipImage = true);
TextureManager textureManager;

// Tilemap
TilemapManager tilemapManager;


int main()
{
    std::cout << colour::yellow << "Warning: app probably has >87346 memory leaks, enjoy =)" << colour::reset << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    
    // Creating the window
    GLFWwindow* window = glfwCreateWindow(windowSettings.width, windowSettings.height, windowSettings.title.c_str(), NULL, NULL);
    if (window == NULL) // If the window fails to create
    {
        std::cout << colour::red << "FAILED to create GLFW window" << colour::reset << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << colour::red << "FAILED to initialize GLAD" << colour::reset << std::endl;
        return -1;
    }

    
    glViewport(0, 0, windowSettings.width, windowSettings.height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    tilemapManager.initTilemap()

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
    
    glfwSetKeyCallback(window, keyCallback);
    
    // #=================# \\ 
    // Main Rendering Loop \\ 
    // #=================# \\ 
    while (!glfwWindowShouldClose(window))
    {
        // Input \\ 
        bool spacePressed = checkKey(GLFW_KEY_SPACE);

        // Rendering \\ 
        // Setting the clear colour
        glClearColor(renderSettings.clearColour[0], renderSettings.clearColour[1], renderSettings.clearColour[2], renderSettings.clearColour[3]);
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
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



