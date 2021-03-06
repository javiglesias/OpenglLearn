#ifndef C__MODEL_H
#define C__MODEL_H

#include <vector>
#include <string>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Shader.h"
namespace Object
{
	class Model
	{
	public:
		Model(){}
		Model(std::string path)
		{
			loadModel(path);
		}
		void Draw(Shader& shader, glm::mat4 model, glm::mat4 view, 
			glm::mat4 projection, glm::vec3 camera_position,
			glm::vec3 light_position, unsigned int texture = -1, unsigned int _instance_count=1);
	private:
		unsigned int total_triangles = 0;
		std::vector<Mesh> meshes;
		std::string directory;
		int width, heigth, nr_channels;
		void loadModel(std::string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial*, aiTextureType, std::string);
		unsigned int TextureFromFile(std::string, std::string);
	public:
		unsigned int triangle_count = 0;
		bool model_loaded = false;
	};
}
#endif // !C__MODEL_H