#pragma once
#ifndef C__MESH_H
#define C__MESH_H
#include "glm.hpp"
#include <vector>
#include "Shader.h"

struct Vertex {
	glm::vec3 position = glm::vec3(0);
	glm::vec3 normal = glm::vec3(0);
	glm::vec2 texcoord = glm::vec2(0);
};
struct Texture {
	unsigned int id = -1;
	std::string type = "";
	std::string path = "";
	int width = -1, heigth = -1, nr_channels = -1;
};
class Mesh 
{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures);
		unsigned int instance_count = 1;
		void Draw(Shader _Shader, glm::mat4 model, glm::mat4 view, 
			glm::mat4 projection, glm::vec3 camera_position,
			glm::vec3 light_position, glm::vec3 _LightColor, int texture = -1, unsigned int _instance_count = 1);
	private:
	// RENDER DATA.
		unsigned int VAO, VBO, EBO;

		void setupMesh();
};
#endif
