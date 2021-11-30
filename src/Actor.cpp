#include "Actor.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

namespace Object
{
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
	void Actor::draw(glm::vec4 _light_ambient, glm::vec4 _light_diffuse, glm::vec4 _light_specular, glm::vec3 _light_directional, 
		glm::vec3 _camera_position, glm::vec3 _light_position, glm::mat4 _projection, glm::mat4 _view)
	{
		//Set the Shader properties for the model
		unsigned int color_location = glGetUniformLocation(shader.id, "RGB_COLOR");
		// TRANSFORMATIONS
		unsigned int light_ambient_location = glGetUniformLocation(shader.id, "dir_light.ambient");
		unsigned int light_diffuse_location = glGetUniformLocation(shader.id, "dir_light.diffuse");
		unsigned int light_specular_location = glGetUniformLocation(shader.id, "dir_light.specular");
		unsigned int light_directional_location = glGetUniformLocation(shader.id, "dir_light.direction");
		unsigned int viewer_position_location = glGetUniformLocation(shader.id, "viewer_position");
		glm::mat4 model(1.f);
		unsigned int model_location = glGetUniformLocation(shader.id, "model");
		unsigned int view_location = glGetUniformLocation(shader.id, "view");
		unsigned int projection_location = glGetUniformLocation(shader.id, "projection");
		//// POINT LIGHT
		unsigned int light_position_location = glGetUniformLocation(shader.id, "point_light[0].position");
		unsigned int material_ambient_location = glGetUniformLocation(shader.id, "material.ambient");
		unsigned int material_diffuse_location = glGetUniformLocation(shader.id, "material.diffuse");
		unsigned int material_specular_location = glGetUniformLocation(shader.id, "material.specular");
		// GENERAL LIGHT SETTINGS
		glUniform3fv(light_ambient_location, 1, glm::value_ptr(_light_ambient));
		glUniform3fv(light_diffuse_location, 1, glm::value_ptr(_light_diffuse));
		glUniform3fv(light_specular_location, 1, glm::value_ptr(_light_specular));
		glUniform3fv(light_directional_location, 1, glm::value_ptr(_light_directional));
		glUniform3fv(viewer_position_location, 1, glm::value_ptr(_camera_position));
		shader.use();
		shader.setFloat("material_shininess", 1);
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(_view));
		//glUniform3fv(light_position_location, 1, glm::value_ptr(Render::light_position));
		glUniform3fv(color_location, 1, glm::value_ptr(shader.rgba_color));
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(_projection));
		for (unsigned int i = 0; i < instace_positions.size(); i++)
		{
			auto current_position = instace_positions.at(i);
			std::string uniform = "instance_positions[" + std::to_string(i)+ "]";
			unsigned int instance_position = glGetUniformLocation(shader.id, uniform.c_str());
			glUniform3fv(instance_position, 1, glm::value_ptr(current_position));
		}
		// RENDER
		model_load.Draw(shader, model, _view, _projection, _camera_position, _light_position);
	}
	void Actor::addInstance(glm::vec3 _position)
	{
		instace_positions.push_back(_position);
	}
}