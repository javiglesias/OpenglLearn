#pragma once
#ifndef C__MESH_H
#define C__MESH_H
#include "glm.hpp"
#include <vector>
#include "Shader.h"
/// <summary>
/// The idea behind the instanced model is to draw all of this models on the Scene in one draw pass.
/// ez, right?
/// So, the thing is that we have to save al vector of positions where we need to draw this instances, right?
/// </summary>
namespace Object
{
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
	};
	struct Texture {
		unsigned int id;
		std::string type;
		std::string path;
		int width, heigth, nr_channels;
	};
	class Instanced_Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		std::vector<glm::vec3> instanced_positions;
		Instanced_Mesh(std::vector<Vertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<Texture> textures);
		void Draw(Shader& shader, glm::mat4 model, glm::mat4 view,
			glm::mat4 projection, glm::vec3 camera_position,
			glm::vec3 light_position, unsigned int texture = -1);
	private:
		// RENDER DATA.
		unsigned int VAO, VBO, EBO;

		void setupMesh();
	};
}
#endif


