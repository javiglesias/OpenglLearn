#include "Mesh.h"
// GLAD goes first
#include "glad/glad.h"
// GLFW goes second
#include "GLFW/glfw3.h" 
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices= vertices;
	this->indices = indices;
	this->textures = textures;
	setupMesh();
}

void Mesh::Draw(Shader& shader, glm::mat4 model, glm::mat4 view, 
	glm::mat4 projection, glm::vec3 camera_position, glm::vec3 light_position,
	unsigned int texture, unsigned int _instance_count)
{
	unsigned int diffuse_Nr = 1;
	unsigned int specular_Nr = 1;
	unsigned int model_location = glGetUniformLocation(shader.id, "model");
	unsigned int view_location = glGetUniformLocation(shader.id, "view");
	unsigned int projection_location = glGetUniformLocation(shader.id, "projection");
	unsigned int viewer_position_location = glGetUniformLocation(shader.id, "viewer_position");
	unsigned int light_position_location = glGetUniformLocation(shader.id, "dir_light.direction");
	instance_count = _instance_count;
	shader.use();
	/*if (texture <= 0)
	{
		glActiveTexture(GL_TEXTURE0);
		shader.setFloat("texture_grass", texture);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	else 
	{*/
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name = textures[i].type;
			if (name._Equal("texture_diffuse"))
			{
				number = std::to_string(diffuse_Nr++);
			}
			else if (name._Equal("texture_specular"))
			{
			}
			shader.setFloat(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
	//}
	glBindVertexArray(VAO);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(viewer_position_location, 1, glm::value_ptr(camera_position));
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(light_position_location, 1, glm::value_ptr(light_position));
	unsigned int instance_position = glGetUniformLocation(shader.id, "instance_positions[1]");
	glm::vec3 v(1, 0, 0);
	glUniform3fv(instance_position, 1, glm::value_ptr(v));
	glm::mat4 projection_tex;
	if (texture >= 0)
	{
		projection_tex = glm::perspective(glm::radians(90.f), (float)495 / 380,
			0.3f, 10.f);
	}
	else if (textures.size() > 0)
	{
		projection_tex = glm::perspective(glm::radians(90.f), (float)textures[0].width / textures[0].heigth,
			0.3f, 10.f);
	}
	else
	{
		projection_tex = glm::perspective(glm::radians(90.f), 1.f,
			0.3f, 10.f);
	}
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection_tex));
	//glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT,0, instance_count);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, & VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
		&vertices[0], GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), 
		&indices[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, texcoord));
	glBindVertexArray(0);
}
