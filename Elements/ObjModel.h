#ifndef OBJMODEL_CLASS_H
#define OBJMODEL_CLASS_H
#include "Mesh.h"
class ObjModel
{
public:
	ObjModel(const char* filename,glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation,std::vector<Texture> _textures);
	void Draw(Shader& shader, Camera& camera);
	glm::mat4 getModelMatrix();
private:
	glm::vec3 position;
	glm::vec3 scale; 
	glm::vec3 rotation;
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;
	void extract_data(const char* filename);
	void assemble(
		std::vector<std::vector<float>> textcoords,
		std::vector<std::vector<float>> vertices,
		std::vector<std::vector<float>> normals,
		std::vector<std::vector<std::vector<float>>> faces
	);
};

#endif
