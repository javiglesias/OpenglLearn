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
		Model(std::string path)
		{
			loadModel(path);
		}
		void Draw(Shader& shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	private:
		std::vector<Mesh> meshes;
		std::string directory;

		void loadModel(std::string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial*, aiTextureType, std::string);
		unsigned int TextureFromFile(std::string, std::string);
	};
}
#endif // !C__MODEL_H