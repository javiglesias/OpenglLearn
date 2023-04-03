#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "glad/glad.h"

Shader::Shader(const char* _Name, const char* _VertexPath, const char* _FragmentPath, const char* _ComputeShader, float* _Color)
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

	name = _Name;
	if (_Color)
	{
		Color.x = _Color[0];
		Color.y = _Color[1];
		Color.z = _Color[2];
	}
	else
		Color = glm::vec4(1.f);
	memcpy(VertexPath, _VertexPath, sizeof(VertexPath));
	memcpy(FragmentPath, _FragmentPath, sizeof(FragmentPath));
	memcpy(ComputePath, _ComputeShader, sizeof(ComputePath));

	try
	{
		v_shader_file.open(_VertexPath);
		f_shader_file.open(_FragmentPath);
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
	int vertex, fragment, compute;
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
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &f_shader_code, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, log_info);
		std::cerr << log_info;
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
	float_t values[3] = {value.x, value.y, value.z};
	glUniform3fv(glGetUniformLocation(id, name.c_str()),1, values);
}

void Shader::setColor(const std::string& name, glm::vec3 value)
{
	float_t values[3] = { value.x, value.y, value.z };
	Color.x = value.x;
	Color.y = value.y;
	Color.z = value.z;
	setVec3(name, value);
}

const char* Shader::GetVertexShaderPath()
{
	return VertexPath;
}

const char* Shader::GetFragmentShaderPath()
{
	return FragmentPath;
}

const char* Shader::GetComputeShaderPath()
{
	return ComputePath;
}
