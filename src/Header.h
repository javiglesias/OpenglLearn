#pragma once
#ifndef HEADER__H
#define HEADER__H

#include "vendor/imgui.h"
#include "vendor/imgui_impl_glfw.h"
#include "vendor/imgui_impl_opengl3.h"
#include "glad/glad.h"
// GLFW goes second
#include "GLFW/glfw3.h" 

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Shader.h"
#include "Model.h"
#include "../dependencies/stb_image/stb_image.h"

#include <iostream>
#include <queue>
#include <thread>
#include <future>

// Rendering Namespace
namespace Render {

	enum GUI_COMMANDS
	{
		Text,
		SameLine
	};
	struct GUI_command
	{
		GUI_COMMANDS command;
		std::string value;

		GUI_command(GUI_COMMANDS _command, std::string _value)
		{
			command = _command;
			value = _value;
		}
	};

	struct model_loaded
	{
		Object::Model model_load;
		Shader shader;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec3 camera_position;
		
		model_loaded(Object::Model _model_load, Shader _shader,
		glm::mat4 _model, glm::mat4 _view, glm::mat4 _projection,
		glm::vec3 _camera_position)
		{
			model_load = _model_load;
			shader = _shader;
			model = _model;
			view = _view;
			projection = _projection;
			camera_position = _camera_position;
		}
	};
	
	#define CHUNK 1000
	float SCALE = 1.f;

	std::queue<GUI_command> gui_commands_q;
	std::queue<model_loaded> models_loaded;

	//DEBUG GUI
	void DEBUG_LOG(std::string title, std::string value)
	{
		gui_commands_q.push(Render::GUI_command(
			Render::GUI_COMMANDS::Text, title.c_str()));
		gui_commands_q.push(Render::GUI_command(
			Render::GUI_COMMANDS::SameLine, ""));
		gui_commands_q.push(GUI_command(
			GUI_COMMANDS::Text, value.c_str()));
	}
	// END DEBUG GUI
	int screen_width = 800, screen_heigth = 600;
	float z_near = 0.3f, z_far = 1000.f;
	glm::vec3 camera_forward;
	float field_of_view;
	float yaw;
	float pitch;
	float last_x_position;
	float last_y_position;
	bool first_mouse_interaction;
	glm::vec4 light_ambient(1.f, 1.f, 1.f, 1.f);
	glm::vec4 light_diffuse(1.f, 1.f, 1.f, 1.f);
	glm::vec4 light_specular(1.f, 1.f, 1.f, 1.f);
	glm::vec3 light_position(5.f);
	glm::vec3 light_directional(0.f, 0.f, 1.f);
	float light_k_constant = 1.0;
	float light_k_linear = 0.09f;
	float light_k_quadratic = 0.032f;
	float shininess = 32.f;
	float light_cutoff = 12.5f;
	float light_outer_cutoff = 17.2f;
	glm::vec3 coral(1.f, .5f, .31f);
	bool VAO_MODE = true;
	glm::vec3 camera_position = glm::vec3(0.f, 0.f, 0.3f);
	glm::vec3 camera_up = glm::vec3(0.f, 1.f, 0.f);
	glm::mat4 view = glm::lookAt(camera_position,
		Render::camera_forward, camera_up);
	float const y_constant = camera_position.y;
	float camera_speed = 0.05f;
	float delta_time = 0.f;
	float last_frame = 0.f;
	float current_frame = glfwGetTime();
	unsigned int cubes_spawned = 0;
	bool demo_mode = false;
	bool demo_model = false;
	bool show_GUI_cursor = true;
	unsigned int squared_world_size = CHUNK;
	bool mouse_movement = false;
	bool directional_light_enabled = false;
	bool point_light_enabled = false;
	bool spot_light_enabled = false;

}

// FREE FUNCTIONS
void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
	glViewport(-1, -1, width, height);
	//glm::ortho(0.f, (float)width, 0.f, (float)height, 0.3f, 10.f);
}

void mouse_movement_callback(GLFWwindow* window, double x_position, double y_position)
{
	if (Render::mouse_movement)
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
}
void mouse_scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	Render::field_of_view -= y_offset;
	if (Render::field_of_view < 1.f) Render::field_of_view = 1.f;
	if (Render::field_of_view > 90.f) Render::field_of_view = 90.f;
}

void process_input(GLFWwindow* m_window)
{
	// MOVIMIENTO DE LA LUZ
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		Render::light_position.z -= 1;
	} else if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		Render::light_position.z += 1;
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		Render::light_position.x += 1;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		Render::light_position.x -= 1;
	}
	// CAMERA MOVEMENT
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Render::camera_position += Render::camera_speed * Render::camera_forward;
		Render::light_position += Render::camera_speed * Render::camera_forward;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Render::camera_position += glm::normalize(glm::cross(
			Render::camera_up, Render::camera_forward)) * Render::camera_speed;
		Render::light_position += glm::normalize(glm::cross(
			Render::camera_up, Render::camera_forward)) * Render::camera_speed;
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Render::camera_position -= Render::camera_speed * Render::camera_forward;
		Render::light_position -= Render::camera_speed * Render::camera_forward;
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Render::camera_position -= glm::normalize(glm::cross(
			Render::camera_up, Render::camera_forward)) * Render::camera_speed;
		Render::light_position -= glm::normalize(glm::cross(
			Render::camera_up, Render::camera_forward)) * Render::camera_speed;
	}
	if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		Render::show_GUI_cursor = !Render::show_GUI_cursor;
	}
	// FPS old school
	// Render::camera_position.y = Render::y_constant;
	if (glfwGetKey(m_window, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		Render::shininess += 1;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_END) == GLFW_PRESS)
	{
		Render::shininess -= 1;
	}
	if (glfwGetKey(m_window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
	{
		Render::SCALE += 0.01f;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
	{
		Render::SCALE -= 0.01f;
	}

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		Render::mouse_movement = true;
	}
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE 
		&& Render::mouse_movement)
	{
		Render::mouse_movement = false;
	}
}
#endif // !HEADER__H