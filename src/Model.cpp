#include "Model.h"
// GLAD goes first
#include "glad/glad.h"
// GLFW goes second
#include "GLFW/glfw3.h" 

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "../dependencies/stb_image/stb_image.h"

void Model::Draw(Shader _Shader, glm::mat4 model, glm::mat4 view, 
	glm::mat4 projection, glm::vec3 camera_position, glm::vec3 light_position,
	glm::vec3 _LightColor, unsigned int texture, int _instance_count, glm::vec3 _Color)
{
	int indices = 0;
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(_Shader, model, view, projection, camera_position, 
			light_position, _LightColor, texture, _instance_count);
		indices += meshes[i].indices.size();
	}
	_Shader.setColor("RgbColor", _Color);
	if (_instance_count > 0)
		glDrawElementsInstanced(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0, _instance_count);
	else
		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
}

bool Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Error loading the model " << importer.GetErrorString() << '\n';
		return false;
	}
	directory = path.substr(0, path.find_last_of('/'));
	directory = directory + '/';
	processNode(scene->mRootNode, scene);
	return true;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int  i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex{};
		try
		{
			vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			if(mesh->mNormals != nullptr)
				vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			if (mesh->mTextureCoords[0])
			{
				vertex.texcoord = glm::vec2(mesh->mTextureCoords[0]->x, mesh->mTextureCoords[0]->y);
			}
			else
			{
				vertex.texcoord = glm::vec2(0.f, 0.f);
			}
		}
		catch (std::exception ex)
		{
			std::cerr << "Error reading Position, normals or texcoords" << ex.what() << '\n';
		}
		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	if (mesh->mMaterialIndex > 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
		std::vector<Texture> specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

	}
	else
	{
		aiMaterial* material{};
	}
	Status = LOADING;
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType texture_type, std::string name)
{
	std::vector<Texture> textures;
	// Hacer un bucle que compruebe si la textura que se intenta cargar, ya estaba cargada en memoria, para optimizar.
	for (unsigned int i = 0; i < mat->GetTextureCount(texture_type); i++)
	{
		aiString str;
		mat->GetTexture(texture_type, i, &str);
		Texture tex;
		tex.id = TextureFromFile(str.C_Str(), directory);
		tex.type = name;
		tex.path = str.C_Str();
		tex.heigth = heigth;
		tex.width = width;
		tex.nr_channels = nr_channels;
		textures.push_back(tex);
	}
	return textures;
}

unsigned int Model::TextureFromFile(std::string str, std::string directory)
{
	stbi_set_flip_vertically_on_load(false);
	directory.append(str);
	unsigned char* texture_data = stbi_load(directory.c_str(),
		&width, &heigth, &nr_channels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE,
		texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data);
	return texture;
}
