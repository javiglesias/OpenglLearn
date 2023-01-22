#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "glad/glad.h"

Shader::Shader(const char* _name, const char* vertex_path, const char* fragment_path, const char* _ComputeShader)
{
	std::string vertex_code;
	std::string fragment_code;
	std::string compute_code;
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;
	std::ifstream c_shader_file;

	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	c_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	name = _name;
	try
	{
		v_shader_file.open(vertex_path);
		f_shader_file.open(fragment_path);
		c_shader_file.open(_ComputeShader);

		std::stringstream v_shader_stream, f_shader_stream, c_shader_stream;
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();
		c_shader_stream << c_shader_file.rdbuf();

		v_shader_file.close();
		f_shader_file.close();
		c_shader_file.close();

		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();
		compute_code = c_shader_stream.str();

	} catch(std::ifstream::failure e)
	{
		std::cerr << e.what();
	}
	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();
	const char* c_shader_code = compute_code.c_str();
	unsigned int vertex, fragment, compute;
	int success;
	char log_info[512];
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v_shader_code, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, log_info);
		std::cerr << log_info;
	}
	else
	{
		std::cout << "Vertex Shader created successfully.\n";
	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &f_shader_code, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, log_info);
		std::cerr << log_info;
	}
	else
	{
		std::cout << "Fragment Shader created successfully.\n";
	}
	// COMPUTE SHADER
	iComputeShaderProgram = glCreateProgram();
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &c_shader_code, NULL);
	glCompileShader(compute);
	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(compute, 512, nullptr, log_info);
		std::cerr << log_info;
	}
	else
	{
		std::cout << "Compute Shader created successfully.\n";
	}
	glAttachShader(iComputeShaderProgram, compute);
	glLinkProgram(iComputeShaderProgram);
	glDeleteShader(compute);

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);

	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, nullptr, log_info);
		std::cerr << log_info;
	}
	else
	{
		std::cout << "Shader program linked successfully.\n";
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(id);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	//glUniform3fv(glGetUniformLocation(id, name.c_str()), value);
}