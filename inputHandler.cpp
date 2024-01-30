#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <OTHER/stb_image.h>

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>

#include <CUSTOM/inputHandler.h>

std::map<int, bool> pressedKeys;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    while (true)
    {
        std::cout << "Key pressed: " << key << std::endl;

        // Closing the window by pressing the escape key
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, true);
            break;
        }

        // Add pressed keys to the vector
        if (action == GLFW_PRESS)
        {
            //pressedKeys[key] = (true);
            pressedKeys[key] = (action == GLFW_PRESS);
        }
        else
        {
            pressedKeys[key] = false;
        }
        std::cout << "State of key: " << pressedKeys[key] << std::endl;
        break;
    }   
}

inline bool checkKey(int key)
{
    return pressedKeys[key];
}
