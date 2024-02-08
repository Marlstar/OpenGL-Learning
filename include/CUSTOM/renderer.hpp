#pragma once

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
		unsigned int VBO;
		unsigned int VAO;

		void createVBO(float* vertices, GLsizeiptr size)
		{
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
			std::cout << colour::bright_green << "INFO | Created VBO" << colour::reset << std::endl;
		}
		void createVAO()
		{
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			std::cout << colour::bright_green << "INFO | Created VAO" << colour::reset << std::endl;
		}

		void bindVBO(float* vertices, GLsizeiptr size)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
			std::cout << colour::bright_green << "INFO | Bound VBO" << colour::reset << std::endl;
		}
		
		void configureVertices()
		{

		}

private:

};