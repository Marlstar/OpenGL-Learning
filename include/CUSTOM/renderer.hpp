#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OTHER/termcolor.hpp"

// Libraries
#include <iostream>
#include <string>
#include <array>
#include <vector>

class Renderer
{
public:
	struct
	{
		unsigned int VBO;
		unsigned int VAO;

		void createVBO(float vertices[])
		{
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			std::cout << colour::bright_green << "INFO | Created VBO" << colour::reset << std::endl;
		}
		void createVAO()
		{
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			std::cout << colour::bright_green << "INFO | Created VAO" << colour::reset << std::endl;
		}

		void bindVBO(float vertices[])
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			std::cout << colour::bright_green << "INFO | Bound VBO" << colour::reset << std::endl;
		}
		
		void configureVertices()
		{

		}
	} vertices;

private:

};