#include "Actor.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

Actor::Actor(glm::mat4 _view, glm::mat4 _projection,
	glm::vec3 _camera_position, bool _visible)
{
	model = glm::mat4(1.f);
	view = _view;
	projection = _projection;
	camera_position = _camera_position;
	memcpy(name, "object", sizeof("object"));
	visible = _visible;
	glm::vec3 position{ 0 };
	instace_positions.push_back(position);
}
Actor::Actor(Model _model_load, uint8_t _ShaderId,
	glm::mat4 _model, glm::mat4 _view, glm::mat4 _projection,
	glm::vec3 _camera_position, const char* _name, const char* _type,
	bool _visible)
{
	model_load = _model_load;
	model = _model;
	view = _view;
	ShaderId = _ShaderId;
	projection = _projection;
	camera_position = _camera_position;
	memcpy(name, _name, sizeof(_name));
	memcpy(type, _type, sizeof(type));
	visible = _visible;
	glm::vec3 position{0};
	instace_positions.push_back(position);
}

Actor::~Actor()
{
}

uint8_t Actor::getShader()
{
	return ShaderId;
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

void Actor::getName(char& name_)
{
	name_ = *name;
}

const char* Actor::getNameConst()
{
	return name;
}

const char* Actor::getTypeConst()
{
	return type;
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
void Actor::Draw(Shader _Shader, glm::vec4 _light_ambient, glm::vec4 _light_diffuse, glm::vec4 _light_specular, glm::vec3 _light_directional, 
	glm::vec3 _camera_position, glm::vec3 _light_position, glm::vec3 _LightColor, glm::mat4 _projection, glm::mat4 _view)
{
	//Set the Shader properties for the model
	
	model_load.Draw(_Shader, model, _view, _projection, _camera_position, _light_position, _LightColor);
}
void Actor::addInstance(glm::vec3 _position)
{
	instace_positions.push_back(_position);
}

void Actor::Serialize()
{
	// TO-DO serializar el objeto a un string para volver a leerlo despues

}
