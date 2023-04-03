
#include "../dependencies/stb_image/stb_image.h"
#include "glad/glad.h"
// GLFW goes second
#include "GLFW/glfw3.h" 
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Render.h"

GLFWwindow* m_window = nullptr;
// Dedicated GPU for laptops with 2 https://stackoverflow.com/questions/16823372/forcing-machine-to-use-dedicated-graphics-card
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
	//glViewport(0.3f, 10.f, width, height);
	//glm::ortho(0.f, (float_t)width, 0.f, (float_t)height, -1.f, 2.f);
}
bool Render_Init(const char* _name, int _ScreenWidth, int _ScreenHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
		GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(_ScreenWidth, _ScreenHeight,
		"LearnOpenGL", nullptr, nullptr);

	if (m_window == nullptr)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Error loading GLAD.");
		return false;
	}
	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);
	//Render prep
	/*glEnable(GL_BLEND);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_STENCIL_TEST);

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glStencilFunc(GL_NOTEQUAL, (GLint)1, 0xff);*/
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	/*framebuffer_size_callback(m_window, _ScreenWidth, _ScreenHeight);*/
	return true;
}

void Render_SetCallbacks(void (*_MouseMovement)(GLFWwindow* window, double x_position, double y_position), 
	void (*_MouseScroll)(GLFWwindow* window, double x_offset, double y_offset))
{
	// CALLBACKS
	glfwSetCursorPosCallback(m_window, _MouseMovement);
	glfwSetScrollCallback(m_window, _MouseScroll);
}

int Render_ShouldCloseWindow()
{
	return glfwWindowShouldClose(m_window);
}

float_t Render_GetTime()
{
	return (float_t)glfwGetTime();
}

void Render_PollEvents()
{
	// poll the events and call the callback functions.
	glfwPollEvents();
}

void Render_SwapBuffers()
{
	// swap the Color buffer
	glfwSwapBuffers(m_window);
}

void Render_Sleep(int _Milisecons)
{
	Sleep(_Milisecons);
}

RENDER_INPUT Render_ProcessInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		return RENDER_INPUT::KEY_W;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		return RENDER_INPUT::KEY_A;
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		return RENDER_INPUT::KEY_S;
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		return RENDER_INPUT::KEY_D;
	}
	// FPS old school
	// vCameraPosition.y = y_constant;

	if (glfwGetKey(m_window, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		return RENDER_INPUT::KEY_HOME;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_END) == GLFW_PRESS)
	{
		return RENDER_INPUT::KEY_END;
	}
	if (glfwGetKey(m_window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
	{
		return RENDER_INPUT::KEY_PAGE_UP;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
	{
		return RENDER_INPUT::KEY_PAGE_DOWN;
	}

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		return RENDER_INPUT::MOUSE_BUTTON_RIGHT;
	}
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		return RENDER_INPUT::MOUSE_BUTTON_LEFT;
	}
	// EDITION MODE
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		return RENDER_INPUT::KEY_R;
	}
}

void Render_ClearColor()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Render_BindVertexArray(unsigned int _VertexArray)
{
	glBindVertexArray(_VertexArray);
}

bool Render_Close()
{
	glfwTerminate();
	return true;
}