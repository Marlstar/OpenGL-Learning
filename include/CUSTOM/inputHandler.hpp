#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <OTHER/stb_image.h>

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>

std::map<int, bool> pressedKeys;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    while (true)
    {
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
        else if (action == GLFW_RELEASE)
        {
            pressedKeys[key] = false;
        }
        break;
    }
}



inline bool checkKey(int key)
{
    return pressedKeys[key];
}
