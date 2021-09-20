#pragma once
#ifndef C__SHADER_H
#define C__SHADER_H

#include "glm.hpp"
#include <string>
class Shader
{
public:
	unsigned int id;
	Shader(){}
	Shader(const char* vertex_path, const char* fragment_path);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setFloat(const std::string &name, float value) const;
	void setInt(const std::string& name, int value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
};
#endif
