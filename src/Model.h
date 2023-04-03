#ifndef C__MODEL_H
#define C__MODEL_H

#include <vector>
#include <string>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Shader.h"
#include "Resource.h"

class Model : public cResource
{
public:
	Model(){}
	Model(std::string path)
	{
		if (loadModel(path)) Status = RESOURCE_STATUS::READY;
	}
	void Draw(Shader _Shader, glm::mat4 model, glm::mat4 view, 
		glm::mat4 projection, glm::vec3 camera_position,
		glm::vec3 light_position, glm::vec3 _LightColor, 
		unsigned int texture = -1, int _instance_count=10, 
		glm::vec3 _Color = glm::vec3(255,255,0));
private:
	std::vector<Mesh> meshes;
	std::string directory;
	int width, heigth, nr_channels;
	bool loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial*, aiTextureType, std::string);
	unsigned int TextureFromFile(std::string, std::string);
};
#endif // !C__MODEL_H