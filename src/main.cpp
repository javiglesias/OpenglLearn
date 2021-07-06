// GLAD goes first
#include "glad/glad.h"
// GLFW goes second
#include "GLFW/glfw3.h" 

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Shader.h"
#include "Model.h"
#include "../dependencies/stb_image/stb_image.h"

#include "Header.h"

int main(int args, char** argv)
{
	Render::first_mouse_interaction = true;
	Render::yaw = -90.f;
	Render::pitch = 0.f;
	Render::camera_forward = glm::vec3(0.f, 0.f, -1.f);
	Render::field_of_view = 90.f;

	float vertices_cube_complete[] = {
		// CUBO COMPLETO
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

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

	// INICIALIZAMOS IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& input_output = ImGui::GetIO();
	(void)input_output;
	// INICIALIZAMOS EL ESTULO DE IMGUI
	ImGui::StyleColorsDark();
	// ESTABLECEMOS LA PATAFORMA Y RENDER
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	// CALLBACKS
	framebuffer_size_callback(m_window, 800, 600);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_window, mouse_movement_callback);
	glfwSetScrollCallback(m_window, mouse_scroll_callback);


	// CREATE SHADER INSTANCE AND PROGRAM
	Shader my_shader("resources/shaders/basic_shader.vs",
		"resources/shaders/basic_shader.fs");

	// VERTEX BUFFER OBJECT (GPU)
	//unsigned int VBO;
	//glGenBuffers(1, & VBO);
	//// VERTEX ARRAY OBJECT
	//unsigned int VAO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	// ELEMENT BUFFER OBJECT
	//unsigned int indices[] = {
	//	0,1,2,
	//	0,2,3
	//};
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//// POSITION FOR VERTEX SHADER (location 0)
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// COLOR FOR VERTEX SHADER (location 1)
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);
	//// TEXTURE COORDINATES FOR THE SHADER
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//// VERTEX BUFFER OBJECT (GPU)
	//unsigned int VBO_triangle;
	//glGenBuffers(1, &VBO_triangle);
	////// VERTEX ARRAY OBJECT
	//unsigned int VAO_triangle;
	//glGenVertexArrays(1, &VAO_triangle);
	//glBindVertexArray(VAO_triangle);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_triangle);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle), vertices_triangle, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	/*unsigned int VBO_cube;
	glGenBuffers(1, &VBO_cube);
	unsigned int VAO_cube;
	glGenVertexArrays(1, &VAO_cube);
	glBindVertexArray(VAO_cube);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube_complete), vertices_cube_complete, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/
	
	unsigned int instanced_cubes;
	glGenBuffers(1, &instanced_cubes);
	glBindBuffer(GL_ARRAY_BUFFER, instanced_cubes);
	glBufferData(GL_ARRAY_BUFFER, CHUNK * sizeof(vertices_cube_complete), NULL, GL_STATIC_DRAW);

	//// Load Texture 1
	stbi_set_flip_vertically_on_load(false);
	int width, heigth, nr_channels;
	unsigned char* texture_data = stbi_load("resources/textures/WoddenBox.png",
		&width, &heigth, &nr_channels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE,
		texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data);

	////// Load Texture 2
	int width2, heigth2, nr_channels2;
	unsigned char* texture_data2 = stbi_load("resources/textures/WoddenBox_specular.png",
		&width2, &heigth2, &nr_channels2, 0);
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, heigth2, 0, GL_RGB, GL_UNSIGNED_BYTE,
		texture_data2);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data2);

	//// CUBO LIGHT SOURCE
	Shader light_shader("resources/shaders/light_shader.vs",
		"resources/shaders/light_shader.fs");

	glm::mat4 model(1.f);
	glm::mat4 light_model(1.f);
	unsigned int light_model_location = glGetUniformLocation(light_shader.id, "model");
	unsigned int light_view_location = glGetUniformLocation(light_shader.id, "view");
	unsigned int light_projection_location = glGetUniformLocation(light_shader.id, "projection");
	//
	unsigned int VBO_light_source;
	glGenBuffers(1, &VBO_light_source);
	unsigned int VAO_light_source;
	glGenVertexArrays(1, &VAO_light_source);
	glBindVertexArray(VAO_light_source);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_light_source);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube_complete), vertices_cube_complete, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//// TRANSFORMATIONS
	glm::mat4 projection = glm::perspective(glm::radians(Render::field_of_view), (float)width / heigth,
		0.1f, 100.f);

	unsigned int model_location = glGetUniformLocation(my_shader.id, "model");
	unsigned int view_location = glGetUniformLocation(my_shader.id, "view");
	unsigned int projection_location = glGetUniformLocation(my_shader.id, "projection");
	//
	//// GENERAL LIGHT SETTINGS
	unsigned int viewer_position_location = glGetUniformLocation(my_shader.id, "viewer_position");
	//// DIRECTIONAL LIGHT
	unsigned int light_ambient_location = glGetUniformLocation(my_shader.id, "dir_light.ambient");
	unsigned int light_diffuse_location = glGetUniformLocation(my_shader.id, "dir_light.diffuse");
	unsigned int light_specular_location = glGetUniformLocation(my_shader.id, "dir_light.specular");
	unsigned int light_directional_location = glGetUniformLocation(my_shader.id, "dir_light.direction");
	//// POINT LIGHT
	unsigned int light_position_location = glGetUniformLocation(my_shader.id, "point_light[0].position");

	
	unsigned int material_ambient_location = glGetUniformLocation(my_shader.id, "material.ambient");
	unsigned int material_diffuse_location = glGetUniformLocation(my_shader.id, "material.diffuse");
	unsigned int material_specular_location = glGetUniformLocation(my_shader.id, "material.specular"); 

	unsigned int rgb_color_location = glGetUniformLocation(my_shader.id, "RGB_COLOR");
	
	glm::mat4 view(1.f);
	view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));
	// CAMERA
	glm::vec3 camera(0.f, 0.f, 0.3f);
	glm::vec3 camera_target(0.f, 0.f, 0.f);
	glm::vec3 camera_direction(glm::normalize(camera - camera_target));
	glm::vec3 up(0.f, 1.f, 0.f);
	// Vector cross product gives us a orthogonal vector;
	glm::vec3 camera_right(glm::normalize(glm::cross(up, Render::camera_forward)));
	// this cross gives us the last direction orthogonal
	glm::vec3 camera_up(glm::cross(camera_direction, camera_right));
	glm::mat4 look_at(
		glm::vec4(camera_right, 0.f),
		glm::vec4(Render::camera_up, 0.f),
		glm::vec4(Render::camera_forward, 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f));
	look_at *= glm::mat4(
		glm::vec4(1.f, 0.f, 0.f, Render::camera_position.x),
		glm::vec4(0.f, 1.f, 0.f, Render::camera_position.y),
		glm::vec4(0.f, 0.f, 1.f, Render::camera_position.z),
		glm::vec4(0.f, 0.f, 0.f, 1.f));


	Render::view = glm::lookAt(Render::camera_position, Render::camera_forward, Render::camera_up); // camera up direction
	std::string model_path = args > 1 ? argv[1] : "resources/models/backpack.obj";
	Shader backpack_shader("resources/shaders/material_shader.vs",
		"resources/shaders/material_shader.fs");
	//Object::Model chest(model_path);
	//Object::Model chest(model_path);
	unsigned int squared_world_size = 16;
	glm::vec3 start_point = glm::vec3(0.f);
	
	//Render prep
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1.f, 0xff);

	//Render loop
	while (!glfwWindowShouldClose(m_window))
	{
		Render::delta_time = Render::current_frame - Render::last_frame;
		Render::last_frame = Render::current_frame;
		
		process_input(m_window);

		Render::DEBUG_LOG("Rendering chunk of size: ", std::to_string(CHUNK).c_str());
		Render::DEBUG_LOG("Triangle count: ", std::to_string(CHUNK * CHUNK * 2 * 6).c_str());
		Render::DEBUG_LOG("Frame time: ", std::to_string(Render::last_frame).c_str());


		//All the rendering things
		glClearColor(0.2f, 0.1f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (Render::show_GUI_cursor)
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else 
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		float time_value = glfwGetTime();
		float green = (sin(time_value)/2.f)+ .5f;
		Render::view = glm::lookAt(Render::camera_position, Render::camera_position + Render::camera_forward,
			Render::camera_up); // camera up direction
		projection = glm::perspective(glm::radians(Render::field_of_view), (float)width / heigth,
		0.1f, 100.f);  // Change de Field of view;
		//
		//// MOVE LIGHT OVER TIME
		/*Render::light_position = glm::vec3(glm::sin(glfwGetTime()),
			glm::sin(glfwGetTime()),
			Render::light_position.z);
		glm::mat4 projection = glm::mat4(1.f);*/
		glBindVertexArray(VAO_light_source);
		light_model = glm::mat4(10.f);
		light_model = glm::scale(light_model, glm::vec3(0.2f));
		light_model = glm::translate(light_model, Render::light_position);
		light_shader.use();
		glUniformMatrix4fv(light_model_location, 1, GL_FALSE, glm::value_ptr(light_model));
		glUniformMatrix4fv(light_view_location, 1, GL_FALSE, glm::value_ptr(Render::view));
		glUniformMatrix4fv(light_projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // a menos que los dos test pase, mantenemos el valor del buffer
		//glStencilFunc(GL_ALWAYS, 1, 0xFF); // a 1 todo
		//glStencilMask(0xFF); // activamos la escritura al buffer

		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices_cube_complete) / sizeof(float));

		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // invertimos la condicion del test queremos los que son != 1
		//glStencilMask(0x00); // desactivamos escritura al buffer.
		//glDisable(GL_DEPTH_TEST);
		//// pintamos otra vez.
		//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices_cube_complete) / sizeof(float));

		//// DEMO MODE
		/*const float radius(10.f);
		float camx(sin(glfwGetTime()) * radius);
		float camz(cos(glfwGetTime()) * radius);
		view = glm::lookAt(
			glm::vec3(camx, 0.f, camz),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f));*/

		//// LINKING UNIFORM SHADER ATTRIBUTES
		my_shader.use();
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		// LIGHT CONFIG
		glUniform3fv(light_ambient_location, 1, glm::value_ptr(Render::light_ambient));
		glUniform3fv(light_diffuse_location, 1, glm::value_ptr(Render::light_diffuse));
		glUniform3fv(light_specular_location, 1, glm::value_ptr(Render::light_specular));
		glUniform3fv(light_directional_location, 1, glm::value_ptr(Render::light_directional));
		// ATTENUATION
		glUniform3fv(light_position_location, 1, glm::value_ptr(Render::light_position));
		my_shader.setFloat("point_light[0].k_constant",Render::light_k_constant);
		my_shader.setFloat("point_light[0].k_linear", Render::light_k_linear);
		my_shader.setFloat("point_light[0].k_quadratic", Render::light_k_quadratic);
		//////// SPOTLIGHT
		//my_shader.setFloat("light.cutoff", glm::cos(glm::radians(Render::light_cutoff)));
		//my_shader.setFloat("light.outer_cutoff", glm::cos(glm::radians(Render::light_outer_cutoff)));

		// OBSERVATOR
		glUniform3fv(viewer_position_location, 1, glm::value_ptr(Render::camera_position));
		//
		// MATERIAL CONFIG
		glUniform3fv(material_ambient_location	, 1, glm::value_ptr(glm::vec3(.5)));
		glUniform3fv(material_diffuse_location	, 1, glm::value_ptr(glm::vec3(1)));
		glUniform3fv(material_specular_location	, 1, glm::value_ptr(glm::vec3(1)));
		
		my_shader.setFloat("material.shininess", Render::shininess);
		my_shader.setInt("material.diffuse_map", 0);
		my_shader.setInt("material.specular_map", 1);
		my_shader.setFloat("CHUNK", CHUNK);
		my_shader.setFloat("SCALE", Render::SCALE);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);*/
		//////// DRAW
		/*for (unsigned int i = 0; i < squared_world_size; i++)
		{
			for (unsigned int j = 0; j < squared_world_size; j++)
			{*/

				model = glm::mat4(1.f);
				/*start_point = glm::vec3(2.1 * vertices_cube_complete[0] * i, 0.f, 2.1 * vertices_cube_complete[0] * j);
				model = glm::translate(model, start_point);*/
				glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(Render::view));
				
				/*glBindVertexArray(VAO_cube);
				glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices_cube_complete) / sizeof(float));*/
			/*}
		}*/
		glUniform3fv(rgb_color_location, 1, glm::value_ptr(glm::vec3(94.f, 157.f, 52.f)));
		glDrawArraysInstanced(GL_TRIANGLES, 0, sizeof(vertices_cube_complete) / sizeof(float), CHUNK* CHUNK);
		glBindVertexArray(0);

		ImGui::Begin("DEBUG LOG");
			while(!Render::gui_commands_q.empty())
			{
				Render::GUI_command command = Render::gui_commands_q.front();
				Render::gui_commands_q.pop();
				switch (command.command)
				{
					case Render::GUI_COMMANDS::Text:
						ImGui::Text(command.value.c_str());
						break;
					case Render::GUI_COMMANDS::SameLine:
						ImGui::SameLine();
						break;
					default:
						break;
				}
			}
		ImGui::End();
		//	APRENDIZAJE DE GRAFICOS y GUI
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Graphics"))
			{
				if (ImGui::MenuItem("Wireframe", "1"))
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				if (ImGui::MenuItem("Solid", "2"))
				{
					glDisable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				if (ImGui::MenuItem("Stencil 0x00", "3"))
				{
					glEnable(GL_STENCIL_BUFFER_BIT);
					glStencilMask(0x00);
					glStencilFunc(GL_EQUAL, 0.5f, 0xff);
				}
				if (ImGui::MenuItem("Disable Stencil", "4"))
				{
					glDisable(GL_STENCIL_BUFFER_BIT);
				}
				if (ImGui::MenuItem("Disable Depth", "5"))
				{
					glDisable(GL_DEPTH_TEST);
				}
				if (ImGui::MenuItem("Enable Depth", "6"))
				{
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LESS);
				}
				if (ImGui::MenuItem("Cull Back", "7"))
				{
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
				}
				if (ImGui::MenuItem("Cull Front", "8"))
				{
					glEnable(GL_CULL_FACE);
					glCullFace(GL_FRONT);
				}
				if (ImGui::MenuItem("Toggle cursor", "TAB"))
				{
					Render::show_GUI_cursor = !Render::show_GUI_cursor;
				}
				if (ImGui::MenuItem("Material Shine +", "HOME"))
				{
					Render::shininess += 1;
				}
				if (ImGui::MenuItem("Material Shine -", "END"))
				{
					Render::shininess -= 1;
				}
				if (ImGui::MenuItem("Scale +", "PAG UP"))
				{
					Render::SCALE += 0.01f;
				}
				if (ImGui::MenuItem("Scale -", "PAG DOWN"))
				{
					Render::SCALE -= 0.01f;
				}
				if (ImGui::MenuItem("Quit", "ESC"))
				{
					glfwSetWindowShouldClose(m_window, true);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// RENDER THE DATA FOR THE GUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		//chest.Draw(backpack_shader, model, Render::view, projection, Render::camera_position);
		// poll the events and call the callback functions.
		glfwPollEvents();
		// swap the Color buffer
		glfwSwapBuffers(m_window);
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
