#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout
			<< "OGL Error: "
			<< error
			<< " : " << function
			<< " : " << file
			<< " : " << line
			<< "\n";
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT))
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	// Modern OpenGl
	// Utilizar los vao tanto como pueda, lo recomienda OpenGl
	shader.Bind();
	va.Bind();
	ib.Bind();
	// Esto siempre tiene que ser unsigned int, la info del draw se tiene que obtener del IndexBuffer
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr))
}
