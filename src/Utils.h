#ifndef C__RENDER
#define C__RENDER

#include "Actor.h"
#include <queue>

#define _(x) #x
#define FRAMECAP60 0.01666666666666667
#define FRAMECAP30 0.03333333333333333
#define CHUNK 1000
#define DEBUG 1
// Rendering Namespace
namespace Render
{
	enum BASIC_SHAPES
	{
		Cube,
		Cone,
		Cylinder,
		Plane,
		Sphere,
		Torus,
		LightBulb,
		Monkey
	};
	struct Shape_model
	{
		BASIC_SHAPES shape;
		Object::Model model_obj;
	};
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
		std::string name;
		bool visible = true;

		model_loaded(Object::Model _model_load, Shader _shader,
			glm::mat4 _model, glm::mat4 _view, glm::mat4 _projection,
			glm::vec3 _camera_position, std::string _name = "model_",
			bool _visible = true)
		{
			model_load = _model_load;
			shader = _shader;
			model = _model;
			view = _view;
			projection = _projection;
			camera_position = _camera_position;
			name = _name;
			visible = _visible;
		}
	};
	float SCALE = 1.f;

	std::queue<GUI_command> gui_commands_q;
	std::vector<Object::Actor> models_loaded;
	std::vector<Object::Actor> static_world;
	std::vector<Object::Actor> dynamic_world;
	std::vector<Object::Actor> lights_loaded;
	std::string temp_custom_model;

	unsigned int screen_width = 1024, screen_heigth = 720;
	float z_near = 0.3f, z_far = 10.f;
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
	glm::vec3 light_position(1.f);
	glm::vec3 light_directional(0.f, 0.f, 1.f);
	float light_k_constant = 1.0;
	float light_k_linear = 0.09f;
	float light_k_quadratic = 0.032f;
	glm::vec3 point_lights_position[4];
	float shininess = 32.f;
	float light_cutoff = 12.5f;
	float light_outer_cutoff = 17.2f;
	glm::vec3 coral(1.f, .5f, .31f);
	bool VAO_MODE = true;
	glm::vec3 camera_position = glm::vec3(0.f, 0.f, 0.3f);
	float go_camera_position[3]{camera_position.x, camera_position.y, camera_position.z};
	glm::vec3 camera_up = glm::vec3(0.f, 1.f, 0.f);
	glm::mat4 view = glm::lookAt(camera_position,
		Render::camera_forward, camera_up);
	glm::mat4 projection = glm::mat4(1.f);
	float const y_constant = camera_position.y;
	float camera_speed = 0.01f;
	float delta_time = 0.f;
	float new_frame = 0.f;
	float time_accumulated = 0.f;
	float time_accumulated_physics = 0.f;
	float frame_cap = FRAMECAP60;
	float current_frame = 0.f;
	unsigned int cubes_spawned = 0;
	bool demo_mode = false;
	bool custom_model = false;
	bool show_GUI_cursor = true;
	unsigned int squared_world_size = CHUNK;
	bool directional_light_enabled = false;
	bool point_light_enabled = false;
	bool spot_light_enabled = false;
	const char *polygon_mode{"Solid"};
	const char* culling_mode{ "ON" };
	const char* stencil_mode{ "ON" };
	const char* depth_mode{ "ON" };
	const char* cull_mode{ "Back" };
	float mat_shine_mode = 32.f;
	float scale_mode = 32.f;
	std::vector<Shader> loaded_shaders{};
}
#endif