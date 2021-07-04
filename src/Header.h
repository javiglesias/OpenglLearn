#pragma once
#ifndef HEADER__H
#define HEADER__H

#include "glad/glad.h"
// GLFW goes second
#include "GLFW/glfw3.h" 

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Shader.h"
#include "Model.h"
#include "../dependencies/stb_image/stb_image.h"

// Rendering Namespace
namespace Render {
	#define CHUNK 1000
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
		std::rand() % 2 >= 1 ? true : false
	};
	unsigned int squared_world_size = CHUNK;
}
#endif // !HEADER__H