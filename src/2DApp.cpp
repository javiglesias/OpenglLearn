#include "2DApp.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int App2D::Run(int args, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_PROFILE);
	GLFWwindow* m_window = glfwCreateWindow(800,600, "LearnOpengl2D",
		NULL, NULL);
	if (m_window == nullptr)
	{
		return -1;
	}
	return 0;
}
