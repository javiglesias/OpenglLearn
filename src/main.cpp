// GLAD goes first
#include "glad/glad.h"
// GLFW goes second
#include "glfw3.h" 

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Shader.h"
#include "../dependencies/stb_image/stb_image.h"

#include <iostream>

namespace Render {
	glm::vec3 camera_forward;
	float field_of_view;
	float yaw;
	float pitch;
	float last_x_position;
	float last_y_position;
	bool first_mouse_interaction;
}
bool VAO_MODE = true;
glm::vec3 camera_position = glm::vec3(0.f, 0.f, 3.f);
glm::vec3 camera_up = glm::vec3(0.f, 1.f, 0.f);
glm::mat4 view = glm::lookAt(camera_position, Render::camera_forward, camera_up);
float camera_speed = 0.05f;
float delta_time = 0.f;
float last_frame = 0.f;
float current_frame = glfwGetTime();
float const y_constant = camera_position.y;
bool dual_passage[10] = {
	std::rand() % 2 >= 1 ? true : false ,
	std::rand() % 2 >= 1 ? true : false ,
	std::rand() % 2 >= 1 ? true : false ,
	std::rand() % 2 >= 1 ? true : false ,
	std::rand() % 2 >= 1 ? true : false ,
	std::rand() % 2 >= 1 ? true : false ,
	std::rand() % 2 >= 1 ? true : false ,
	std::rand() % 2 >= 1 ? true : false ,
	std::rand() % 2 >= 1 ? true : false ,
	std::rand() % 2 >= 1 ? true : false };
const char* vertex_shader_source =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 color;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"   color = aColor;\n"
	"}\0";
const char* fragment_shader_source =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 ourColor;\n"
	"in vec3 color;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(color, 1.0f);\n"
	"}\0";

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
	glViewport(-1, -1, width, height);
	//glm::ortho(0.f, (float)width, 0.f, (float)height, 0.3f, 10.f);
}

void mouse_movement_callback(GLFWwindow* window, double x_position, double y_position)
{
	if (Render::first_mouse_interaction)
	{
	//	last_x_position = x_position;
	//	last_y_position = y_position;
		//Render::first_mouse_interaction = false;
	}
	float x_offset = x_position - Render::last_x_position;
	float y_offset = Render::last_y_position - y_position;
	Render::last_x_position = x_position;
	Render::last_y_position = y_position;
	float senseo = 0.1f;
	x_offset *= senseo;
	y_offset *= senseo;
	Render::yaw += x_offset;
	Render::pitch += y_offset;
	// CONSTRAINTS
	if (Render::pitch > 89.0f) Render::pitch = 89.0f;
	if (Render::pitch < -89.0f) Render::pitch = -89.0f;
	glm::vec3 camera_direction;
	camera_direction.x = cos(glm::radians(Render::yaw)) * cos(glm::radians(Render::pitch));
	camera_direction.y = sin(glm::radians(Render::pitch));
	camera_direction.z = sin(glm::radians(Render::yaw)) * cos(glm::radians(Render::pitch));
	Render::camera_forward = glm::normalize(camera_direction);
}
void mouse_scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	Render::field_of_view -= y_offset;
	if (Render::field_of_view < 1.f) Render::field_of_view = 1.f;
	if (Render::field_of_view > 90.f) Render::field_of_view = 90.f;
}

void process_input(GLFWwindow* m_window)
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, true);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_3) == GLFW_PRESS)
	{
		VAO_MODE = false;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_4) == GLFW_PRESS)
	{
		VAO_MODE = true;
	}
	// CAMERA MOVEMENT
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_position += camera_speed * Render::camera_forward;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_position += glm::normalize(glm::cross(
			camera_up, Render::camera_forward)) * camera_speed;
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_position -= camera_speed * Render::camera_forward;
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_position -= glm::normalize(glm::cross(
			camera_up, Render::camera_forward)) * camera_speed;
	}
	camera_position.y = y_constant;
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		// RESET
		for (int i = 0; i < sizeof(dual_passage) / sizeof(bool); i++)
		{
			dual_passage[i] = std::rand() % 2 >= 1 ? true : false;
		}
	}
	if (glfwGetKey(m_window, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		camera_speed += 0.01;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_END) == GLFW_PRESS)
	{
		camera_speed -= camera_speed > 0.01 ? 0.01 : 0.f;
	}
}

int main()
{
	Render::first_mouse_interaction = true;
	Render::yaw = -90.f;
	Render::pitch = 0.f;
	Render::camera_forward = glm::vec3(0.f, 0.f, -1.f);
	Render::field_of_view = 90.f;

	float vertices[] = {
		-0.5f, -0.5f, 1.0f, 0.5f, 0.f, 0.0f,  1.f, 1.f,
		 0.5f, -0.5f, 1.0f, 0.5f, 0.5f, 0.0f, 1.f, 0.f,
		 0.5f, 0.5f, 1.0f,  0.f, 0.5f, 0.0f,  0.f, 0.f,
		-0.5f, 0.5f, 1.0f,  0.5f, 0.5f, 0.0f, 0.f, 1.f
	};

	float vertices_roof_floor[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	float vertices_cube[] = {
		// CHUNK PASILLO
			/*-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,*/

			/*-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,*/

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions(0.0f, 0.0f, 0.0f);

	float vertices_triangle[] = {
		0.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		.5f, 1.f, 0.f
	};
	float tex_coordinates[] = {
		0.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		.5f, 1.f, 0.f
	};
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
		GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* m_window = glfwCreateWindow(800, 600,
		"LearnOpenGL", nullptr, nullptr);
	if (m_window == nullptr)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error loading GLAD.";
		return -1;
	}
	else
	{
		std::cout << "Glad loaded.\n";
	}
	// CALLBACK FOR RESIZE WINDOW
	//framebuffer_size_callback(m_window, 800, 600);
	//glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_window, mouse_movement_callback);
	glfwSetScrollCallback(m_window, mouse_scroll_callback);
	// DEPTH TEST
	glEnable(GL_DEPTH_TEST);
	// CULLING 
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);*/
	// CREATE SHADER INSTANCE AND PROGRAM
	Shader my_shader("resources/shaders/basic_shader.vs",
		"resources/shaders/basic_shader.fs");

	// VERTEX BUFFER OBJECT (GPU)
	/*unsigned int VBO;
	glGenBuffers(1, & VBO);*/
	// VERTEX ARRAY OBJECT
	/*unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);*/
	// ELEMENT BUFFER OBJECT
	/*unsigned int indices[] = {
		0,1,2,
		0,2,3
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/// POSITION FOR VERTEX SHADER (location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// COLOR FOR VERTEX SHADER (location 1)
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);*/
	// TEXTURE COORDINATES FOR THE SHADER
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// VERTEX BUFFER OBJECT (GPU)
	//unsigned int VBO_triangle;
	//glGenBuffers(1, &VBO_triangle);
	//// VERTEX ARRAY OBJECT
	//unsigned int VAO_triangle;
	//glGenVertexArrays(1, &VAO_triangle);
	//glBindVertexArray(VAO_triangle);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_triangle);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle), vertices_triangle, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	unsigned int VBO_cube;
	glGenBuffers(1, &VBO_cube);
	unsigned int VAO_cube;
	glGenVertexArrays(1, &VAO_cube);
	glBindVertexArray(VAO_cube);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// COLOR FOR VERTEX SHADER (location 1)
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);*/
	// TEXTURE COORDINATES FOR THE SHADER
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Load Texture 1
	stbi_set_flip_vertically_on_load(false);
	int width, heigth, nr_channels;
	unsigned char* texture_data = stbi_load("resources/textures/container.jpg",
		&width, &heigth, &nr_channels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE,
		texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data);


	// roof && floor
	unsigned int VBO_roof_floor;
	glGenBuffers(1, &VBO_roof_floor);
	unsigned int VAO_roof_floor;
	glGenVertexArrays(1, &VAO_roof_floor);
	glBindVertexArray(VAO_roof_floor);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_roof_floor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_roof_floor), vertices_roof_floor, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Load Texture 2
	int width2, heigth2, nr_channels2;
	unsigned char* texture_data2 = stbi_load("resources/textures/awesomeface.png",
		&width2, &heigth2, &nr_channels2, 0);
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, heigth2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		texture_data2);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data2);

	// TRANSFORMATIONS
	glm::mat4 projection = glm::perspective(glm::radians(Render::field_of_view), (float)width / heigth,
		0.1f, 100.f);
	glm::mat4 model(1.f);
	unsigned int model_location = glGetUniformLocation(my_shader.id, "model");
	unsigned int view_location = glGetUniformLocation(my_shader.id, "view");
	unsigned int projection_location = glGetUniformLocation(my_shader.id, "projection");
	/*glm::mat4 view(1.f);
	view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));*/
	// CAMERA
	glm::vec3 camera(0.f, 0.f, 0.3f);
	glm::vec3 camera_target(0.f, 0.f, 0.f);
	//glm::vec3 camera_direction(glm::normalize(camera - camera_target));
	glm::vec3 up(0.f, 1.f, 0.f);
	// Vector cross product gives us a orthogonal vector;
	glm::vec3 camera_right(glm::normalize(glm::cross(up, Render::camera_forward)));
	// this cross gives us the last direction orthogonal
	//glm::vec3 camera_up(glm::cross(camera_direction, camera_right));
	glm::mat4 look_at(
		glm::vec4(camera_right, 0.f),
		glm::vec4(camera_up, 0.f),
		glm::vec4(Render::camera_forward, 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f));
	look_at *= glm::mat4(
		glm::vec4(1.f, 0.f, 0.f, camera_position.x),
		glm::vec4(0.f, 1.f, 0.f, camera_position.y),
		glm::vec4(0.f, 0.f, 1.f, camera_position.z),
		glm::vec4(0.f, 0.f, 0.f, 1.f));


	view = glm::lookAt(camera_position, Render::camera_forward, camera_up); // camera up direction



	//Render loop
	while (!glfwWindowShouldClose(m_window))
	{
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		process_input(m_window);
		//All the rendering things
		glClearColor(0.2f, 0.1f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*float time_value = glfwGetTime();
		float green = (sin(time_value)/2.f)+ .5f;*/

		view = glm::lookAt(camera_position, camera_position + Render::camera_forward,
			camera_up); // camera up direction
		projection = glm::perspective(glm::radians(Render::field_of_view), (float)width / heigth,
			0.1f, 100.f); // Change de Field of view;
		// DEMO MODE
		/*const float radius(10.f);
		float camx(sin(glfwGetTime()) * radius);
		float camz(cos(glfwGetTime()) * radius);
		view = glm::lookAt(
			glm::vec3(camx, 0.f, camz),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f));*/
			// LINKING VERTEX ATTRIBUTES
		my_shader.use();
		//my_shader.setFloat("green", green);
		my_shader.setInt("texture1", 0);
		my_shader.setInt("texture2", 1);
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		/*glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/

		glBindVertexArray(VAO_cube);
		for (int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.f);
			model = glm::translate(model, glm::vec3(cubePositions.x,
				y_constant, cubePositions.z + i));
			if (i % 2 == 0)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture2);
				glBindVertexArray(0);
				glBindVertexArray(VAO_roof_floor);
				glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices_roof_floor) / sizeof(float));

				glBindTexture(GL_TEXTURE_2D, texture2);
				glBindVertexArray(0);
				glBindVertexArray(VAO_cube);
				model = glm::translate(model, glm::vec3(cubePositions.x - 1,
					y_constant, cubePositions.z));
				model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
				// MIRROR pAssage
				if (dual_passage[i])
				{
					glm::mat4 model2(glm::mat4(1.f));
					model2 = glm::translate(model2, glm::vec3(cubePositions.x + 1,
						y_constant, cubePositions.z + i));
					model2 = glm::rotate(model2, glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
					glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model2));
					glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices_cube) / sizeof(float));
				}
			}
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices_cube) / sizeof(float));
		}
		glBindVertexArray(0);

		// poll the events and call the callback functions.
		glfwPollEvents();
		// swap the Color buffer
		glfwSwapBuffers(m_window);
	}

	return 0;
}