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
namespace Object
{
	enum TYPE
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
	class Actor
	{
	public:
		std::vector<glm::vec3> instace_positions{};
		bool visible = true;
		
		Actor() {}
		Actor(glm::mat4 _view, glm::mat4 _projection,
			glm::vec3 _camera_position, bool _visible);
		Actor(Object::Model _model_load, Shader _shader,
			glm::mat4 _model, glm::mat4 _view, glm::mat4 _projection,
			glm::vec3 _camera_position, std::string _name = "model_",
			bool _visible = true);
		~Actor();
		Shader& getShader();
		glm::mat4& getProjection();
		glm::mat4& getModel();
		std::string getName();
		Object::Model& getModelLoaded();
		void setModel(glm::mat4 _model);
		float* getPosition();
		void setPosition(float _x, float _y, float _z);
		float* getRotation();
		void setRotation(float _x, float _y, float _z, float rotation);
		void draw(glm::vec4 light_ambient, glm::vec4 light_diffuse, glm::vec4 light_specular, glm::vec3 light_directional, 
			glm::vec3 camera_position, glm::vec3 light_position, glm::mat4 _projection, glm::mat4 _view);
		void addInstance(glm::vec3 _position = glm::vec3(0));
		
	private:
		Model model_load;
		Shader shader;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec3 camera_position;
		std::string name;
		TYPE actor_type;
		float world_position[3]{ 0 };
		float world_rotation[4]{ 0 };
	};
}
#endif // !_H__RENDER
