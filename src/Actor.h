#pragma once
#ifndef _H__RENDER
#define _H__RENDER

#include "Shader.h"
#include "Model.h"
#include "gtc/matrix_transform.hpp"

/// <summary>
/// One actor can have more than one instance?
/// The limit rait nao is 4096 instances of each actor.
/// </summary>

class Actor
{
public:
	std::vector<glm::vec3> instace_positions{};
	bool visible = true;
	bool editable = false;
		
	Actor() {}
	Actor(glm::mat4 _view, glm::mat4 _projection,
		glm::vec3 _camera_position, bool _visible);
	Actor(Model* _model_load, uint8_t _ShaderId,
		glm::mat4 _model, glm::mat4 _view, glm::mat4 _projection,
		glm::vec3 _camera_position, const char* _name = "model_",
		const char* _type = "Cube",
		bool _visible = true);
	~Actor();
	uint8_t getShader();
	glm::mat4& getProjection();
	glm::mat4& getModel();
	void getName(char&);
	const char* getNameConst();
	const char* getTypeConst();
	Model& getModelLoaded();
	void setModel(glm::mat4 _model);
	float* getPosition();
	void setPosition(float _x, float _y, float _z);
	float* getRotation();
	void setRotation(float _x, float _y, float _z, float rotation);
	void Draw(Shader _Shader, glm::vec3 _camera_position, glm::vec3 _light_position, 
		glm::vec3 _LightColor, glm::mat4 _projection, glm::mat4 _view);
	void addInstance(glm::vec3 _position = glm::vec3(0));
	void Serialize();
		
private:
	Model* model_load;
	uint8_t ShaderId;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 camera_position;
	char name[128] = "Actor", type[64];
	float world_position[3]{ 0 };
	float world_rotation[4]{ 0 };
};
#endif // !_H__RENDER
