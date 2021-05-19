#pragma once
#include <string>
#include "GL/glew.h"
#include <unordered_map>

struct ShaderPrograms
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	std::string m_Filepath;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	unsigned int m_RendererID;
	ShaderPrograms source;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniforms1i(const std::string& name, int v0);
	void SetUniforms4f(const std::string& name, float v0, float v1, float f2, float f3);
private:
	unsigned int CompileShader(GLenum type, std::string source);
	unsigned CreateShader(const std::string& vertex_shader, const std::string& framentShader);
	ShaderPrograms ParseShader(const std::string& filepath);
	int GetUniformLocation(const std::string& name);
};
