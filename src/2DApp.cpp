#include "2DApp.h"

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
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Error loading GLAD.");
		return -1;
	}

	m_current_frame = glfwGetTime();
	while (!glfwWindowShouldClose(m_window)) // Game loop
	{
		m_new_frame = glfwGetTime();
		m_delta_time = m_new_frame - m_current_frame;
		m_accumulated_time += m_delta_time;
		m_accumulated_time_physics += m_delta_time;
		if (m_accumulated_time > m_frame_cap) // Render frame
		{
			fprintf(stdout, "\nGame: %f", 1/m_accumulated_time);
			fprintf(stdout, "\nFrame: %d", m_frame_number);
			m_accumulated_time = 0.f;
			m_frame_number++;
			//Render Things
			glClearColor(1.f, 0.f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glViewport(0, 0, 800, 600);

		}
		if (m_accumulated_time_physics > m_frame_cap_physics) // Physics frame
		{
			fprintf(stdout, "\nPhysics: %f", 1/m_accumulated_time_physics);
			m_accumulated_time_physics = 0.f;
			//calculate physics of things

		}	
		m_current_frame = glfwGetTime();
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}
	return 0;
}
