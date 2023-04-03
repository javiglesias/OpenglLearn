#pragma once
#ifndef C__SHADER_H
#define C__SHADER_H

#include "glm.hpp"
#include <string>

class Shader
{
public:
	int id = -1, iComputeShaderProgram = -1;
	glm::vec4 Color;
	const char* name="";
	Shader(){}
	Shader(const char* _name, const char* vertex_path, const char* fragment_path, const char* _ComputeShader = "", float* _Color = nullptr);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setFloat(const std::string &name, float value) const;
	void setInt(const std::string& name, int value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setColor(const std::string& name, glm::vec3 value);
	const char* GetVertexShaderPath();
	const char* GetFragmentShaderPath();
	const char* GetComputeShaderPath();
private:
	char VertexPath[64]   = "";
	char FragmentPath[64] = "";
	char ComputePath[64]  = "";
};
#endif
