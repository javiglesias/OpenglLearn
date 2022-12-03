#include "Actor.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

Actor::Actor(glm::mat4 _view, glm::mat4 _projection,
	glm::vec3 _camera_position, bool _visible)
{
	Model Lightbulb("resources/models/BasicShapes/LightBulb.obj");
	Shader light_shader("resources/shaders/light_shader.vert",
		"resources/shaders/light_shader.frag", "light_shader");
	model_load = Lightbulb;
	shader = light_shader;
	model = glm::mat4(1.f);
	view = _view;
	projection = _projection;
	camera_position = _camera_position;
	name = "object";
	visible = _visible;
	glm::vec3 position{ 0 };
	instace_positions.push_back(position);
}
Actor::Actor(Model _model_load, Shader _shader,
	glm::mat4 _model, glm::mat4 _view, glm::mat4 _projection,
	glm::vec3 _camera_position, std::string _name,
	bool _visible)
{
	model_load = _model_load;
	shader = _shader;
	model = _model;
	view = _view;
	projection = _projection;
	camera_position = _camera_position;
	name = _name;
	visible = _visible;
	glm::vec3 position{0};
	instace_positions.push_back(position);
}

Actor::~Actor()
{
}

Shader& Actor::getShader()
{
	return shader;
}

glm::mat4& Actor::getProjection()
{
	return projection;
}

glm::mat4& Actor::getModel()
{
	// TODO: Insertar una instrucción "return" aquí
	return model;
}

void Actor::setModel(glm::mat4 _model)
{
	model = _model;
}

Model& Actor::getModelLoaded()
{
	// TODO: Insertar una instrucción "return" aquí
	return model_load;
}

std::string Actor::getName()
{
	return name;
}

float* Actor::getPosition()
{
	return world_position;
}

void Actor::setPosition(float _x, float _y, float _z)
{
	world_position[0] = _x;
	world_position[1] = _y;
	world_position[2] = _z;
	setModel(glm::translate(getModel(),
		glm::vec3(_x, _y, _z)));
}

float* Actor::getRotation()
{
	return world_rotation;
}

void Actor::setRotation(float _x, float _y, float _z, float rotation)
{
	world_rotation[0] = _x;
	world_rotation[1] = _y;
	world_rotation[2] = _z;
	world_rotation[3] = rotation;
	glm::rotate(getModel(), glm::radians(rotation),
		glm::vec3(_x, _y, _z));
}
void Actor::draw(glm::vec4 _light_ambient, glm::vec4 _light_diffuse, glm::vec4 _light_specular, glm::vec3 _light_directional, 
	glm::vec3 _camera_position, glm::vec3 _light_position, glm::mat4 _projection, glm::mat4 _view)
{
	//Set the Shader properties for the model
	unsigned int color_location = glGetUniformLocation(shader.id, "RGB_COLOR");
	glUniform3fv(color_location, 1, glm::value_ptr(shader.rgba_color));
	model_load.Draw(shader, model, _view, _projection, _camera_position, _light_position);
}
void Actor::addInstance(glm::vec3 _position)
{
	instace_positions.push_back(_position);
}