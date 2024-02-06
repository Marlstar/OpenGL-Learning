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
	int width = 0;
	int height = 0;
	std::string title = "Default Title";

	Window(int width_, int height_, std::string title_)
	{
		width = width_;
		height = height_;
		title = title_;
		
		createWindow();
	}

	void createWindow()
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
		glViewport(0, 0, width, height);
	}
private:
	
};