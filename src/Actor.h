#pragma once
#ifndef _H__RENDER
#define _H__RENDER

#include "Shader.h"
#include "Model.h"
#include "gtc/matrix_transform.hpp"

class Actor
{
public:
	Actor(Object::Model _model_load, Shader _shader,
		glm::mat4 _model, glm::mat4 _view, glm::mat4 _projection,
		glm::vec3 _camera_position, std::string _name = "model_",
		bool _visible = true);
	~Actor();
	Shader& getShader();
	glm::mat4& getProjection();
	glm::mat4& getModel();
	void setModel(glm::mat4 _model);
	Object::Model& getModelLoaded();
	std::string getName();
	float* getPosition();
	void setPosition(float _x, float _y, float _z);
	float* getRotation();
	void setRotation(float _x, float _y, float _z, float rotation);

	bool visible = true;
private:
	Object::Model model_load;
	Shader shader;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 camera_position;
	std::string name;
	float world_position[3] { 0 };
	float world_rotation[4] { 0 };
};
#endif // !_H__RENDER
