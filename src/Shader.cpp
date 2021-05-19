#include "Shader.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	:m_RendererID(0), m_Filepath(filepath)
{
	source = ParseShader(filepath);
	m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID))
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0))
}

void Shader::SetUniforms1i(const std::string& name, int v0)
{
	int location = Shader::GetUniformLocation(name);
	GLCall(glUniform1i(location, v0))
}

void Shader::SetUniforms4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int location = Shader::GetUniformLocation(name);
	GLCall(glUniform4f(location, v0, v1, v2, v3))
}

unsigned int Shader::CompileShader(GLenum type, std::string source)
{
	GLCall(unsigned int id = glCreateShader(type))
	// Source tiene que existir cuando se llame a este codigo porque se esta devolviendo la direccion a la que apunta.
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr))
	GLCall(glCompileShader(id))

	int compilationResult;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &compilationResult))
	if (compilationResult == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length))
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message))
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertex_shader, const std::string& framentShader)
{
	GLCall(unsigned int program = glCreateProgram())
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, framentShader);
	GLCall(glAttachShader(program, vertexShader))
	GLCall(glAttachShader(program, fShader))
	GLCall(glLinkProgram(program))
	GLCall(glValidateProgram(program))

	GLCall(glDeleteShader(vertexShader))
	GLCall(glDeleteShader(fShader))

	return program;
}

ShaderPrograms Shader::ParseShader(const std::string& filepath)
{
	std::fstream fShader(filepath);
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	std::string line;
	while (getline(fShader, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

int Shader::GetUniformLocation(const std::string& name)
{
	// Creamos una cache para que devuelva el location sin tene que buscarlo siempre.
	if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	m_UniformLocationCache[name] = location;
	return location;
}
