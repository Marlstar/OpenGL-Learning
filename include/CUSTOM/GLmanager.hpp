#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <OTHER/termcolor.hpp>

// Libraries
#include <iostream>
#include <string>
#include <array>
#include <vector>

class GLmanager
{
public:
	// Initialisation functions
	void initGL()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	}
	bool checkGlad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << colour::red << "FAILED to initialize GLAD" << colour::reset << std::endl;
			return false;
		}
		return true;
	}
};