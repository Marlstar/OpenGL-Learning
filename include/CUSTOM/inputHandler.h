#pragma once

extern std::map<int, bool> pressedKeys;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


inline bool checkKey(int key)
{
    return pressedKeys[key];
}
