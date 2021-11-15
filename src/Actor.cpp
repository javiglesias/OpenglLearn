#include "Actor.h"

namespace Object
{
	Actor::Actor(Object::Model _model_load, Shader _shader,
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

	Object::Model& Actor::getModelLoaded()
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
}