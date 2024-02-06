#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <OTHER/termcolor.hpp>

// Libraries
#include <iostream>
#include <string>
#include <array>
#include <vector>

class Window
{
public:
	GLFWwindow* window;

	void createWindow(int width, int height, std::string title)
	{
		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (window == NULL) // If the window fails to create
		{
			std::cout << colour::red << "FAILED to create GLFW window" << colour::reset << std::endl;
			glfwTerminate();
			return;
		}
	}
	void initialConfiguration()
	{

	}
private:
	
};