#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <SHADERCLASS/shader.h>

#include <iostream>
#include <string>
#include <array>
#include <vector>


// Window settings
struct {
    int width = 800;
    int height = 600;
    std::string title = "the pinnacle of epicness";    
} windowSettings;

// Rendering settings
struct {
    float clearColour[4] = { 0.53f, 0.81f, 0.92f, 1.0f }; // RGBA, scaled from 0-1 rather than 0-255
} renderSettings;

// Resize the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Input
void processInput(GLFWwindow* window);


int main()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    
    // Creating the window
    GLFWwindow* window = glfwCreateWindow(windowSettings.width, windowSettings.height, windowSettings.title.c_str(), NULL, NULL);
    if (window == NULL) // If the window fails to create
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    
    glViewport(0, 0, windowSettings.width, windowSettings.height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    // #============================# \\ 
    // Vertices & Vertex Shader Stuff \\ 
    // #============================# \\ 

    float vertices[] = {
    -0.9f, 0.9f, 0.0f,
    -0.9f, -0.9f, 0.0f,
    0.9f, 0.9f, 0.0f,

    0.9f, 0.9f, 0.0f,
    -0.9f, -0.9f, 0.0f,
    0.9f, -0.9f, 0.0f
    };

    // Vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Shader basicShader("SHADERS/VertexShader.glsl", "SHADERS/FragmentShader.glsl");

    // Tell openGL how to interpret the vertex data we have given it
    // Each vertex has 3 points, and each of those points is represented by a 4-byte float
    // 
    // #= Vertex 1 =# #= Vertex 2 =# #= Vertex 3 =#
    // XXXX YYYY ZZZZ XXXX YYYY ZZZZ XXXX YYYY ZZZZ
    // ==>4 ==>8 =>12 =>16 =>20 =>24 =>28 =>32 =>36

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // VAO (Vertex Array Object)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    // Bind to the buffer containing the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    
    // #=================# \\ 
    // Main Rendering Loop \\ 
    // #=================# \\ 
    while (!glfwWindowShouldClose(window))
    {
        // Input \\ 
        processInput(window);

        // Rendering \\ 
        // Setting the clear colour
        glClearColor(renderSettings.clearColour[0], renderSettings.clearColour[1], renderSettings.clearColour[2], renderSettings.clearColour[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        // Drawing the triangles =)
        int numTriangles;
        numTriangles = sizeof(vertices) / sizeof(float) / 3;
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

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}