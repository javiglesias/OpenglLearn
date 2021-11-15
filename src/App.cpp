#include "App.h"
#include <iostream>
#include <fstream>
#include "Utils.h"

// FREE FUNCTIONS
void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
	glViewport(-1, -1, width, height);
	//glm::ortho(0.f, (float)width, 0.f, (float)height, 0.3f, 10.f);
}

void mouse_movement_callback(GLFWwindow* window, double x_position, double y_position)
{
	if (!Render::show_GUI_cursor)
	{
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
	if (Render::field_of_view > 90.f) Render::field_of_view = 100.f;
}

void process_input(GLFWwindow* m_window)
{
	// MOVIMIENTO DE LA LUZ
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		Render::light_position.z -= 1;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
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
	/*if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		Render::show_GUI_cursor = !Render::show_GUI_cursor;
	}*/
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
		Render::show_GUI_cursor = false;
	}
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		Render::show_GUI_cursor = true;
	}
}
std::string ExePath() {
	char pwd[256]{};
	return std::string(pwd);
}

int App::Run(int args, char** argv,unsigned int mode) // mode Debug:0 Retail:1 
{
	bool is_set_position_open = false;
	unsigned int frame_number{ 0 };

	Render::first_mouse_interaction = true;
	Render::yaw = -90.f;
	Render::pitch = 0.f;
	Render::camera_forward = glm::vec3(0.f, 0.f, -1.f);
	Render::field_of_view = 90.f;
	GetModelsOnFolder();
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

	GLFWwindow* m_window = glfwCreateWindow(Render::screen_width, Render::screen_heigth,
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

#if DEBUG
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
#endif

	// CALLBACKS
	framebuffer_size_callback(m_window, Render::screen_width, Render::screen_heigth);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_movement_callback);
	glfwSetScrollCallback(m_window, mouse_scroll_callback);
	glEnable(GL_BLEND);

	// CREATE SHADER INSTANCE AND PROGRAM
	Shader basic_shader("resources/shaders/basic_shader.vert",
		"resources/shaders/basic_shader.frag", "basic_shader");
	Render::loaded_shaders.push_back(basic_shader);
	Shader basic_shape_shader("resources/shaders/basic_shape_shader.vert",
		"resources/shaders/basic_shape_shader.frag", "basic_shape_shader");
	Render::loaded_shaders.push_back(basic_shape_shader);
	Shader light_shader("resources/shaders/light_shader.vert",
		"resources/shaders/light_shader.frag", "light_shader");
	Render::loaded_shaders.push_back(light_shader);
	Shader backpack_shader("resources/shaders/material_shader.vert",
		"resources/shaders/material_shader.frag", "material_shader");
	Render::loaded_shaders.push_back(backpack_shader);
	Shader grass_shader("resources/shaders/grass_shader.vert",
		"resources/shaders/grass_shader.frag", "grass_shader");
	Render::loaded_shaders.push_back(grass_shader);
	//// VERTEX BUFFER OBJECT (GPU)
	//unsigned int VBO;
	//glGenBuffers(1, & VBO);
	////// VERTEX ARRAY OBJECT
	//unsigned int VAO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle), vertices_triangle, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//// ELEMENT BUFFER OBJECT
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

	////// VERTEX BUFFER OBJECT (GPU)
	//unsigned int VBO_triangle;
	//glGenBuffers(1, &VBO_triangle);
	//////// VERTEX ARRAY OBJECT
	//unsigned int VAO_triangle;
	//glGenVertexArrays(1, &VAO_triangle);
	//glBindVertexArray(VAO_triangle);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_triangle);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle), vertices_triangle, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//unsigned int VBO_cube;
	//glGenBuffers(1, &VBO_cube);
	//unsigned int VAO_cube;
	//glGenVertexArrays(1, &VAO_cube);
	//glBindVertexArray(VAO_cube);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube_complete), vertices_cube_complete, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	////FRAMEBUFFERS object
	//unsigned int fbo;
	//glGenFramebuffers(1, &fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	std::cout << "Error creating framebuffer." << '\n';
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//	glDeleteFramebuffers(1, &fbo);
	//}

	/*unsigned int instanced_cubes;
	glGenBuffers(1, &instanced_cubes);
	glBindBuffer(GL_ARRAY_BUFFER, instanced_cubes);
	glBufferData(GL_ARRAY_BUFFER, CHUNK * sizeof(vertices_cube_complete), NULL, GL_STREAM_DRAW);*/

	//// Load Texture 1
	//stbi_set_flip_vertically_on_load(false);
	//int width, heigth, nr_channels;
	//unsigned char* texture_data = stbi_load("resources/textures/Grass.png",
	//	&width, &heigth, &nr_channels, 0);
	//unsigned int texture_grass;
	//glGenTextures(1, &texture_grass);
	//glBindTexture(GL_TEXTURE_2D, texture_grass);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	//	texture_data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//stbi_image_free(texture_data);

	////// Load Texture 2
	/*int width2, heigth2, nr_channels2;
	unsigned char* texture_data2 = stbi_load("resources/textures/WoddenBox_specular.png",
		&width2, &heigth2, &nr_channels2, 0);
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, heigth2, 0, GL_RGB, GL_UNSIGNED_BYTE,
		texture_data2);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data2);*/

	/*unsigned int VBO_light_source;
	glGenBuffers(1, &VBO_light_source);
	unsigned int VAO_light_source;
	glGenVertexArrays(1, &VAO_light_source);
	glBindVertexArray(VAO_light_source);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_light_source);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube_complete), vertices_cube_complete, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/

	//// TRANSFORMATIONS
	Render::projection = glm::perspective(glm::radians(Render::field_of_view),
		(float)Render::screen_width / Render::screen_heigth, Render::z_near, Render::z_far);
	glm::mat4 model(1.f);
	unsigned int model_location = glGetUniformLocation(Render::loaded_shaders[0].id, "model");
	unsigned int view_location = glGetUniformLocation(basic_shader.id, "view");
	unsigned int projection_location = glGetUniformLocation(Render::loaded_shaders[0].id, "projection");
	//
	//// GENERAL LIGHT SETTINGS
	unsigned int viewer_position_location = glGetUniformLocation(Render::loaded_shaders[0].id, "viewer_position");
	//// DIRECTIONAL LIGHT
	unsigned int light_ambient_location = glGetUniformLocation(Render::loaded_shaders[0].id, "dir_light.ambient");
	unsigned int light_diffuse_location = glGetUniformLocation(Render::loaded_shaders[0].id, "dir_light.diffuse");
	unsigned int light_specular_location = glGetUniformLocation(Render::loaded_shaders[0].id, "dir_light.specular");
	unsigned int light_directional_location = glGetUniformLocation(Render::loaded_shaders[0].id, "dir_light.direction");
	//// POINT LIGHT
	unsigned int light_position_location = glGetUniformLocation(Render::loaded_shaders[0].id, "point_light[0].position");


	unsigned int material_ambient_location = glGetUniformLocation(Render::loaded_shaders[0].id, "material.ambient");
	unsigned int material_diffuse_location = glGetUniformLocation(Render::loaded_shaders[0].id, "material.diffuse");
	unsigned int material_specular_location = glGetUniformLocation(Render::loaded_shaders[0].id, "material.specular");

	unsigned int rgb_color_location = glGetUniformLocation(Render::loaded_shaders[0].id, "RGB_COLOR");

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


	//Render::view = glm::lookAt(Render::camera_position, Render::camera_forward, Render::camera_up); // camera up direction
	std::string model_path = args > 1 ? argv[1] : "resources/models/backpack.obj";
	//Object::Model chest(model_path);
	unsigned int squared_world_size = 16;
	glm::vec3 start_point = glm::vec3(0.f);

	//Render prep
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1.f, 0xff);

	// Cargando modelos basicos. BASIC SHAPES
	Object::Model Lightbulb("resources/models/BasicShapes/LightBulb.obj");
	Object::Model Cube("resources/models/BasicShapes/Cube.obj");
	Object::Model Monkey("resources/models/BasicShapes/Monkey.obj");
	Object::Model Cone("resources/models/BasicShapes/Cone.obj");
	Object::Model Plane("resources/models/BasicShapes/Plane.obj");
	Object::Model Torus("resources/models/BasicShapes/Torus.obj");
	Object::Model Sphere("resources/models/BasicShapes/Sphere.obj");
	Object::Model Cylinder("resources/models/BasicShapes/Cylinder.obj");
	Object::Model Labyrinth("resources/models/laberinth.obj");

	//bool rendering_light_model = false;

	// INITIAL LIGHT
	/*unsigned int light_model_location = glGetUniformLocation(light_shader.id, "model");
	unsigned int light_view_location = glGetUniformLocation(light_shader.id, "view");
	unsigned int light_projection_location = glGetUniformLocation(light_shader.id, "projection");

	glm::mat4 light_model(1.f);
	light_model = glm::scale(light_model, glm::vec3(0.2f));
	light_model = glm::translate(light_model, Render::light_position);
	Actor light_to_load(Lightbulb, light_shader,
		light_model, Render::view, Render::projection,
		Render::camera_position, "Light");*/
	//Render::lights_loaded.push_back(light_to_load);
	//Render::models_loaded.push_back(light_to_load);

	// fixed timestep
	Render::current_frame = glfwGetTime();
	//Render loop
	while (!glfwWindowShouldClose(m_window))
	{
		Render::new_frame = glfwGetTime();
		Render::delta_time = Render::new_frame - Render::current_frame;
		Render::current_frame = Render::new_frame;
		Render::time_accumulated += Render::delta_time;
		Render::time_accumulated_physics += Render::delta_time;
		// Frame fixed a 60
		if (Render::time_accumulated > Render::frame_cap)
		{
#if DEBUG
			// IMGUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
#endif
			frame_number++;
			process_input(m_window);

			/*Render::DEBUG_LOG("Rendering chunk of size: ", std::to_string(CHUNK).c_str());
			Render::DEBUG_LOG("Triangle count: ", std::to_string(CHUNK * CHUNK * 2 * 6).c_str());*/

			//All the rendering things
			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#if DEBUG
			// Clean the accumulated because we render this frame.
			ImGui::LabelText("Fps", std::to_string(1 / Render::time_accumulated).c_str());
			ImGui::LabelText("Frame", std::to_string(frame_number).c_str());
#endif
			Render::time_accumulated = 0;

			if (Render::show_GUI_cursor)
			{
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}

			float time_value = glfwGetTime();
			float green = (sin(time_value) / 2.f) + .5f;
			Render::view = glm::lookAt(Render::camera_position, Render::camera_position + Render::camera_forward,
				Render::camera_up); // camera up direction
			/*Render::projection = glm::perspective(glm::radians(Render::field_of_view), (float)width / heigth,
			0.1f, 100.f);*/  // Change de Field of view;
			//
			//// MOVE LIGHT OVER TIME
			/*Render::light_position = glm::vec3(glm::sin(glfwGetTime()),
				glm::sin(glfwGetTime()),
				Render::light_position.z);
			glm::mat4 projection = glm::mat4(1.f);*/
			//glBindVertexArray(VAO_light_source);
			//light_shader.use();
			/*glUniformMatrix4fv(light_model_location, 1, GL_FALSE, glm::value_ptr(light_model));
			glUniformMatrix4fv(light_view_location, 1, GL_FALSE, glm::value_ptr(Render::view));
			glUniformMatrix4fv(light_projection_location, 1, GL_FALSE, glm::value_ptr(projection));
			*///glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // a menos que los dos test pase, mantenemos el valor del buffer
			//glStencilFunc(GL_ALWAYS, 1, 0xFF); // a 1 todo
			//glStencilMask(0xFF); // activamos la escritura al buffer
			//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices_cube_complete) / sizeof(float));

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
				//if (Render::demo_mode)
				//{
				//	basic_shader.use();
				//	// LIGHT CONFIG
				//	if (Render::directional_light_enabled)
				//	{
				//		glUniform3fv(light_ambient_location, 1, glm::value_ptr(Render::light_ambient));
				//		glUniform3fv(light_diffuse_location, 1, glm::value_ptr(Render::light_diffuse));
				//		glUniform3fv(light_specular_location, 1, glm::value_ptr(Render::light_specular));
				//		glUniform3fv(light_directional_location, 1, glm::value_ptr(Render::light_directional));
				//	}
				//	// ATTENUATION
				//	if (Render::point_light_enabled)
				//	{
				//		glUniform3fv(light_position_location, 1, glm::value_ptr(Render::light_position));
				//		basic_shader.setFloat("point_light[0].k_constant", Render::light_k_constant);
				//		basic_shader.setFloat("point_light[0].k_linear", Render::light_k_linear);
				//		basic_shader.setFloat("point_light[0].k_quadratic", Render::light_k_quadratic);

				//	}

				//	// MATERIAL CONFIG
				//	if (Render::spot_light_enabled)
				//	{
				//		//basic_shader.setFloat("light.cutoff", glm::cos(glm::radians(Render::light_cutoff)));
				//		//basic_shader.setFloat("light.outer_cutoff", glm::cos(glm::radians(Render::light_outer_cutoff)));
				//	
				//		glUniform3fv(material_ambient_location, 1, glm::value_ptr(glm::vec3(.5)));
				//		glUniform3fv(material_diffuse_location, 1, glm::value_ptr(glm::vec3(1)));
				//		glUniform3fv(material_specular_location, 1, glm::value_ptr(glm::vec3(1)));

				//		basic_shader.setFloat("material.shininess", Render::shininess);
				//		basic_shader.setInt("material.diffuse_map", 0);
				//		basic_shader.setInt("material.specular_map", 1);
				//	}

				//	basic_shader.setFloat("CHUNK", CHUNK);
				//	basic_shader.setFloat("SCALE", Render::SCALE);

				//	model = glm::mat4(1.f);
				//	model = glm::translate(model, glm::vec3(1.f));
				//	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
				//	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
				//	// OBSERVATOR
				//	glUniform3fv(viewer_position_location, 1, glm::value_ptr(Render::camera_position));
				//	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(Render::view));
				//	glUniform3fv(rgb_color_location, 1, glm::value_ptr(glm::vec3(94.f, 157.f, 52.f)));
				//	
				//	glDrawArraysInstanced(GL_TRIANGLES, 0, sizeof(vertices_cube_complete) / sizeof(float), CHUNK * CHUNK);

				//}
				//if (Render::cubes_spawned > 0)
				//{
				//	unsigned int model_location = glGetUniformLocation(basic_shape_shader.id, "model");
				//	unsigned int view_location = glGetUniformLocation(basic_shape_shader.id, "view");
				//	unsigned int projection_location = glGetUniformLocation(basic_shape_shader.id, "projection");
				//	//
				//	//// GENERAL LIGHT SETTINGS
				//	unsigned int viewer_position_location = glGetUniformLocation(basic_shape_shader.id, "viewer_position");
				//	//// DIRECTIONAL LIGHT
				//	unsigned int light_ambient_location = glGetUniformLocation(basic_shape_shader.id, "dir_light.ambient");
				//	unsigned int light_diffuse_location = glGetUniformLocation(basic_shape_shader.id, "dir_light.diffuse");
				//	basic_shape_shader.use();
				//	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
				//	basic_shape_shader.setFloat("material_shininess", 1);
				//	glUniform3fv(viewer_position_location, 1, glm::value_ptr(Render::camera_position));
				//	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
				//	glUniform3fv(light_position_location, 1, glm::value_ptr(Render::light_position));
				//	glm::mat4 projection_tex;
				//	projection_tex = glm::perspective(glm::radians(90.f), 1.f,
				//		0.3f, 10.f);
				//	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection_tex));

				///*	glUniform3fv(material_ambient_location, 1, glm::value_ptr(glm::vec3(.5)));
				//	glUniform3fv(material_diffuse_location, 1, glm::value_ptr(glm::vec3(1)));
				//	glUniform3fv(material_specular_location, 1, glm::value_ptr(glm::vec3(1)));*/

					/*basic_shader.setFloat("CHUNK", Render::cubes_spawned);
					basic_shader.setFloat("SCALE", Render::SCALE);*/
				//	glDrawArraysInstanced(GL_TRIANGLES, 0, sizeof(vertices_cube_complete) / sizeof(float), Render::cubes_spawned);
				//	glBindVertexArray(0);
				//}
				/*if (Render::custom_model)
				{
					Render::DEBUG_LOG("Triangles Model loaded: ", std::to_string(Render::custom_model.triangle_count).c_str());
					for (auto custom_model : Render::custom_models)
					{

					}
					Render::custom_model.Draw(basic_shape_shader, model, Render::view, projection, Render::camera_position,
						Render::light_directional);
				}*/
#if DEBUG
			ImGui::Begin("World outliner");
#endif
			if (Render::lights_loaded.size() > 0)
			{
				for (auto it = 0; it < Render::lights_loaded.size(); it++)
				{
					bool to_delete = false;
					auto light_to_draw = Render::lights_loaded.at(it);
					static int current_outliner = 0;
#if DEBUG
					if (ImGui::SmallButton("X"))
					{
						to_delete = true;
					}
					ImGui::SameLine();
					if (ImGui::MenuItem(light_to_draw.getName().c_str(), "", &Render::lights_loaded[it].visible))
					{
					}
					if (ImGui::BeginPopupContextItem())
					{
						ImGui::Text("Current position:");
						std::string current_position = std::to_string(light_to_draw.getPosition()[0]) + ","
							+ std::to_string(light_to_draw.getPosition()[1]) + ","
							+ std::to_string(light_to_draw.getPosition()[2]);
						ImGui::Text(current_position.c_str());
						ImGui::Separator();
						float position[3]{};
						ImGui::InputFloat3("Translate", position);
						float rotate_axis[3]{};
						ImGui::InputFloat3("Rotation axis", rotate_axis);
						float rotation{ 0.f };
						ImGui::InputFloat("Rotate", &rotation);
						if (ImGui::Button("OK"))
						{
							Render::lights_loaded[it].setPosition(position[0], position[1], position[2]);
							Render::lights_loaded[it].setRotation(rotate_axis[0], rotate_axis[1], rotate_axis[2], rotation);
						}
						ImGui::SameLine();
						if (ImGui::Button("Cancel"))
						{
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
#endif
					if (to_delete)
					{
						// Remove the item from the vector
						Render::lights_loaded.erase(Render::lights_loaded.begin() + it);
						// Skip the draw call.
						continue;
					}
					if (Render::lights_loaded[it].visible)
					{
						Render::loaded_shaders[0].setFloat("material.shininess", Render::shininess);
						Render::loaded_shaders[0].setInt("material.diffuse_map", 0);
						Render::loaded_shaders[0].setInt("material.specular_map", 1);
						light_to_draw.getModelLoaded().Draw(light_to_draw.getShader(), light_to_draw.getModel(),
							Render::view, light_to_draw.getProjection(), Render::camera_position,
							Render::light_position);
					}
				}
			}
			glBindVertexArray(0);
			if (Render::models_loaded.size() > 0)
			{
				for (auto it = 0; it < Render::models_loaded.size(); it++)
				{
					bool to_delete = false;
					auto model_to_draw = Render::models_loaded.at(it);
					static int current_outliner = 0;
#if DEBUG
					if (ImGui::SmallButton("X"))
					{
						to_delete = true;
					}
					ImGui::SameLine();
					if (ImGui::MenuItem(model_to_draw.getName().c_str(), "", &Render::models_loaded[it].visible))
					{
					}
					if (ImGui::BeginPopupContextItem())
					{
						ImGui::Text("Current position:");
						std::string current_position = std::to_string(model_to_draw.getPosition()[0]) + ","
							+ std::to_string(model_to_draw.getPosition()[1]) + ","
							+ std::to_string(model_to_draw.getPosition()[2]);
						ImGui::Text(current_position.c_str());
						ImGui::Separator();
						float position[3]{};
						ImGui::InputFloat3("Translate", position);
						float rotate_axis[3]{};
						ImGui::InputFloat3("Rotation axis", rotate_axis);
						float rotation{ 0.f };
						ImGui::InputFloat("Rotate", &rotation);
						if (ImGui::Button("OK"))
						{
							Render::models_loaded[it].setPosition(position[0], position[1], position[2]);
							Render::models_loaded[it].setRotation(rotate_axis[0], rotate_axis[1], rotate_axis[2], rotation);
						}
						ImGui::SameLine();
						if (ImGui::Button("Cancel"))
						{
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
#endif
					if (to_delete)
					{
						Render::models_loaded.erase(Render::models_loaded.begin() + it);
						continue;
					}
					if (Render::models_loaded[it].visible)
					{					
						//Set the Shader properties for the model
						unsigned int color_location = glGetUniformLocation(model_to_draw.getShader().id, "RGB_COLOR");
						glUniform3fv(color_location, 1, glm::value_ptr(model_to_draw.getShader().rgba_color));

						model_to_draw.getModelLoaded().Draw(model_to_draw.getShader(), model_to_draw.getModel(),
							Render::view, model_to_draw.getProjection(), Render::camera_position,
							Render::light_position);
					}
				}
			}
			glBindVertexArray(0);
#if DEBUG
			ImGui::End();
#endif
			/*ImGui::Begin("DEBUG LOG");
				for(unsigned int i=0; i<Render::gui_commands_q.size(); i++)
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
					Render::gui_commands_q.push(command);
				}
			ImGui::End();*/
			//	APRENDIZAJE DE GRAFICOS y GUI
#if DEBUG
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::BeginMenu("Load model", models.size() > 0))
					{
						for (auto model : models)
						{
							if (ImGui::SmallButton("->"))
							{
								auto path = argv[0];
								char* next_token;
								int str_length = sizeof path;
								system("explorer resources\\models\\");
							}
							ImGui::SameLine();
							if (ImGui::MenuItem(model.c_str()))
							{
								Render::temp_custom_model = model;
								Render::custom_model = true;
							}
						}
						ImGui::EndMenu();
					}
					if (ImGui::MenuItem("Demo mode", nullptr, false, false))
					{
						Render::demo_mode = !Render::demo_mode;
					}
					if (ImGui::MenuItem("Quit"))
					{
						glfwSetWindowShouldClose(m_window, true);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
			ImGui::Begin("Tools");
				if (ImGui::ArrowButton("left", ImGuiDir_Left))
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					Render::polygon_mode = "Wireframe";
				}
				ImGui::SameLine();
				ImGui::Text("%s", Render::polygon_mode);
				ImGui::SameLine();
				if (ImGui::ArrowButton("right", ImGuiDir_Right))
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					Render::polygon_mode = "Solid";
				}
				if (ImGui::ArrowButton("Culling ON", ImGuiDir_Left))
				{
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
					//glFrontFace(GL_CCW); //This shows the model inside-out backfaces CW
				}
				ImGui::SameLine();
				ImGui::Text("Culling");
				ImGui::SameLine();
				if (ImGui::ArrowButton("Culling OFF", ImGuiDir_Right))
				{
					glDisable(GL_CULL_FACE);
				}
				if (ImGui::ArrowButton("Stencil 0x00", ImGuiDir_Left))
				{
					glEnable(GL_STENCIL_BUFFER_BIT);
					glStencilMask(0x00);
					glStencilFunc(GL_EQUAL, 0.5f, 0xff);
				}
				ImGui::SameLine();
				ImGui::Text("Stencil");
				ImGui::SameLine();
				if (ImGui::ArrowButton("Disable Stencil", ImGuiDir_Right))
				{
					glDisable(GL_STENCIL_BUFFER_BIT);
				}
				if (ImGui::ArrowButton("Disable Depth", ImGuiDir_Left))
				{
					glDisable(GL_DEPTH_TEST);
				}
				ImGui::SameLine();
				ImGui::Text("Depth");
				ImGui::SameLine();
				if (ImGui::ArrowButton("Enable Depth", ImGuiDir_Right))
				{
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LESS);
				}
				if (ImGui::ArrowButton("Cull Back", ImGuiDir_Left))
				{
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
				}
				ImGui::SameLine();
				ImGui::Text("Cull Front/back");
				ImGui::SameLine();
				if (ImGui::ArrowButton("Cull Front", ImGuiDir_Right))
				{
					glEnable(GL_CULL_FACE);
					glCullFace(GL_FRONT);
				}
				if (ImGui::Button("Toggle cursor"))
				{
					Render::show_GUI_cursor = !Render::show_GUI_cursor;
				}
				if (ImGui::ArrowButton("Material Shine +", ImGuiDir_Up))
				{
					Render::shininess += 1;
				}
				ImGui::SameLine();
				ImGui::Text("Shine");
				ImGui::SameLine();
				if (ImGui::ArrowButton("Material Shine -", ImGuiDir_Down))
				{
					Render::shininess -= 1;
				}
				if (ImGui::ArrowButton("Scale +", ImGuiDir_Up))
				{
					Render::SCALE += 0.01f;
				}
				ImGui::SameLine();
				ImGui::Text("scale");
				ImGui::SameLine();
				if (ImGui::ArrowButton("Scale -", ImGuiDir_Down))
				{
					Render::SCALE -= 0.01f;
				}
				if (ImGui::Button("Frame cap 30"))
				{
					Render::frame_cap = FRAMECAP30;
				}
				if (ImGui::Button("Frame cap 60"))
				{
					Render::frame_cap = FRAMECAP60;
				}
				if (ImGui::Button("step by step"))
				{
					Render::frame_cap = 1;
				}
				if (ImGui::MenuItem("Directional light", "", Render::directional_light_enabled))
				{
					Render::directional_light_enabled = !Render::directional_light_enabled;
				}
				if (ImGui::MenuItem("Spotlight", "", Render::point_light_enabled))
				{
					Render::point_light_enabled = !Render::point_light_enabled;
				}
				if (ImGui::MenuItem("Point light", "", Render::spot_light_enabled))
				{
					Render::spot_light_enabled = !Render::spot_light_enabled;
				}
				if (ImGui::Button("Basic Shape"))
				{
					is_set_position_open = true;
				}
			ImGui::End();
			if (is_set_position_open)
			{
				if (ImGui::Begin("Position"))
				{
					ImGui::Text("Set the position and rotation to spawn:");
					Render::Shape_model model_to_create;
					ImGui::Separator();
					static float position[3];
					ImGui::InputFloat3("position", position);
					static float rotation[1];
					ImGui::InputFloat("rotation angles degrees", rotation);
					static int radio_button;
					if (ImGui::RadioButton("Cube", &radio_button, Render::BASIC_SHAPES::Cube))
					{
						model_to_create.shape = Render::BASIC_SHAPES::Cube;
						model_to_create.model_obj = Cube;
					}
					else if (ImGui::RadioButton("Cone", &radio_button, Render::BASIC_SHAPES::Cone))
					{
						model_to_create.shape = Render::BASIC_SHAPES::Cone;
						model_to_create.model_obj = Cone;
					}
					else if (ImGui::RadioButton("Cylinder", &radio_button, Render::BASIC_SHAPES::Cylinder))
					{
						model_to_create.shape = Render::BASIC_SHAPES::Cylinder;
						model_to_create.model_obj = Cylinder;
					}
					else if (ImGui::RadioButton("Plane", &radio_button, Render::BASIC_SHAPES::Plane))
					{
						model_to_create.shape = Render::BASIC_SHAPES::Plane;
						model_to_create.model_obj = Plane;
					}
					else if (ImGui::RadioButton("Torus", &radio_button, Render::BASIC_SHAPES::Torus))
					{
						model_to_create.shape = Render::BASIC_SHAPES::Torus;
						model_to_create.model_obj = Torus;
					}
					else if (ImGui::RadioButton("LightBulb", &radio_button, Render::BASIC_SHAPES::LightBulb))
					{
						model_to_create.shape = Render::BASIC_SHAPES::LightBulb;
						model_to_create.model_obj = Lightbulb;
					}
					else if (ImGui::RadioButton("Monkey", &radio_button, Render::BASIC_SHAPES::Monkey))
					{
						model_to_create.shape = Render::BASIC_SHAPES::Monkey;
						model_to_create.model_obj = Monkey;
					}
					else if (ImGui::RadioButton("Sphere", &radio_button, Render::BASIC_SHAPES::Sphere))
					{
						model_to_create.shape = Render::BASIC_SHAPES::Sphere;
						model_to_create.model_obj = Sphere;
					}
					if (ImGui::Button("OK"))
					{
						// TODO: rotation of the model.
						glm::mat4 basic_cube_model(1.f);
						basic_cube_model = glm::translate(basic_cube_model,
							glm::vec3(position[0], position[1], position[2]));
						glm::rotate(basic_cube_model, glm::radians(rotation[0]),
							glm::vec3(position[0], position[1], position[2]));
						Object::Model model_to_load;
						std::string actor_name = "";
						switch (model_to_create.shape)
						{
						case Render::BASIC_SHAPES::Cube:
							model_to_load = Cube;
							actor_name = _(Cube);
							break;
						case Render::BASIC_SHAPES::Cylinder:
							model_to_load = Cylinder;
							actor_name = _(Cylinder);
							break;
						case Render::BASIC_SHAPES::Cone:
							model_to_load = Cone;
							actor_name = _(Cone);
							break;
						case Render::BASIC_SHAPES::Plane:
							model_to_load = Plane;
							actor_name = _(Plane);
							break;
						case Render::BASIC_SHAPES::Sphere:
							model_to_load = Sphere;
							actor_name = _(Sphere);
							break;
						case Render::BASIC_SHAPES::Torus:
							model_to_load = Torus;
							actor_name = _(Torus);
							break;
						case Render::BASIC_SHAPES::LightBulb:
							model_to_load = Lightbulb;
							actor_name = _(Lightbulb);
							break;
						case Render::BASIC_SHAPES::Monkey:
							model_to_load = Monkey;
							actor_name = _(Monkey);
							break;
						default:
							model_to_load = Monkey;
							actor_name = _(Monkey);
							break;
						}
						Object::Actor object_to_load(model_to_load, basic_shape_shader,
							basic_cube_model, Render::view, Render::projection,
							Render::camera_position, actor_name);
						Render::models_loaded.push_back(object_to_load);
						is_set_position_open = false;
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel"))
					{
						is_set_position_open = false;
					}
					ImGui::End();
				}
			}
			if (Render::custom_model)
			{
				if (ImGui::Begin("Position"))
				{
					ImGui::Text("Set the position and rotation to spawn:");
					ImGui::Separator();
					float position[3];
					ImGui::InputFloat3("position", position, "%.2f");
					float rotate_axis[3];
					ImGui::InputFloat3("Rotation axis", rotate_axis, "%.2f");
					float rotation = 0.f;
					ImGui::InputFloat("Degrees", &rotation);
					static int current_shader_idx = 0;
					Shader current_shader_selected = Render::loaded_shaders[current_shader_idx];
					const char* current_shader_name = current_shader_selected.name;
					if (ImGui::BeginCombo("Shader to apply", current_shader_name, NULL))
					{
						for (int i = 0 ; i<Render::loaded_shaders.size(); i++)
						{
							auto shader = Render::loaded_shaders[i];
							const bool is_selected = (current_shader_idx == i);
							if (ImGui::Selectable(shader.name, is_selected))
							{
								current_shader_idx = i;
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					static ImVec4 rgba_color = ImVec4(0.f, 0.f,0.f, 0.f);
					if (ImGui::TreeNode("Color pick shader"))
					{
						ImGui::ColorEdit3("rgb color", (float*)&rgba_color, NULL);
						ImGui::TreePop();
					}
					float scale_vec[3];
					ImGui::InputFloat3("Scale", scale_vec);
					if (ImGui::Button("OK"))
					{
						auto temp_mesh = Object::Model("resources\\models\\" + Render::temp_custom_model);
						glm::mat4 basic_cube_model(1.f);
						// TODO: Escalar el modelo que se instancia
						basic_cube_model = glm::scale(basic_cube_model, glm::vec3(scale_vec[0], scale_vec[1], scale_vec[2]));
						current_shader_selected.rgba_color = glm::vec4(rgba_color.x, rgba_color.y, rgba_color.z, rgba_color.w);
						basic_cube_model = glm::rotate(basic_cube_model, glm::radians(rotation), glm::vec3(rotate_axis[0], rotate_axis[1], rotate_axis[2]));
						Object::Actor object_to_load(temp_mesh, current_shader_selected,
							basic_cube_model, Render::view, Render::projection,
							Render::camera_position, Render::temp_custom_model);
						object_to_load.setPosition(position[0], position[1], position[2]);
						Render::models_loaded.push_back(object_to_load);
						Render::custom_model = false;
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel"))
					{
						Render::custom_model = false;
					}
					ImGui::End();
				}
			}
			// RENDER THE DATA FOR THE GUI
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // DEBUG
			// poll the events and call the callback functions.
			glfwPollEvents();
			// swap the Color buffer
			glfwSwapBuffers(m_window);
		}
		if (Render::time_accumulated_physics > FRAMECAP30)
		{
			Render::time_accumulated_physics = 0;
		}
	}
#if DEBUG
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
#endif
    return 0;
}

void App::GetModelsOnFolder()
{
	std::string line{};
	std::ifstream models_paths;
	models = std::vector<std::string>();
	system("resources\\bats\\seachModels.bat");
	models_paths.open("resources\\models\\.models.temp", std::fstream::in);
	while (models_paths.is_open() && models_paths.good() && !models_paths.eof())
	{
		try
		{
			char c = (char)models_paths.get();
			if (c != '\n')
			{
				line += c;
			}
			else
			{
				std::cout << line << '\n';
				models.push_back(line);
				line = "";
			}
		}
		catch (std::exception ex)
		{
			std::cerr << "Cannot find models." << '\n';
		}
	}
}
