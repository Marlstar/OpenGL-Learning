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
		}
		void createVAO()
		{
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
		}

		void bindVBO(float vertices[])
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
	} vertices;

private:

};