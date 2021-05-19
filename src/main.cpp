/// Como siempre en C++ el orden de la importacion afecta
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

struct Player
{
	double x = 0.0, y = 0.0, z = 0.0;
	double shape[4][3] = {
		{1, 1 + y, 1.0}, // Esquina inferior izq
		{1, 10 + y, 1.0}, // Esquina superior izq
		{3, 10 + y, 1.0}, // Esquina superior der
		{3, 1 + y, 1.0} // Esquina inferior der
	};
	double* GetShape()
	{
		return &shape[0][0];
	}
};

struct my_struct // BItfield
{
	unsigned int field1: 2;
	unsigned int field2: 4;
	unsigned int field3 : 8;
	unsigned int field4 : 16;
	unsigned int field5 : 32;
};

struct ShaderProgramSrc
{
	std::string vertexSource;
	std::string fragmentSource;
};

void const read3dObj(std::string);
unsigned int CreateShader(const std::string& vertex_shader, const std::string& framentShader);
unsigned int CompileShader(GLenum type, std::string source);
ShaderProgramSrc ParseShader(const std::string& filepath);

int main(void)
{
	if(!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(640, 640, "H3ll0 W0r1d!", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		return -1;
	}
	Player p1;
	double speed = 3;
	glfwMakeContextCurrent(window);
	
	// We need a valid context to call glewInit
	if (glewInit() != GLEW_OK)
		return -2;
	int windowWidth, windowHeigh;
	glfwGetWindowSize(window, &windowWidth, &windowHeigh);
	read3dObj("resources\\text.txt");
	p1.y = 0;
	{
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};
		
		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		// Texture coordinates.
		
		VertexArray vertexArray;
		VertexBuffer vertexBuffer(vertices, sizeof(vertices));
		
		VertexBufferLayout bufferLayout;
		bufferLayout.Push<float>(2);
		bufferLayout.Push<float>(2);
		vertexArray.AddBuffer(vertexBuffer, bufferLayout);
		
		IndexBuffer indexBuffer(indices, 6);

		Shader shader("resources\\shaders\\Basic.shader");
		std::string uniform = "u_Color";
		Renderer renderer;

		Texture texture("resources\\textures\\Whispers.png");
		int slot = 0;
		texture.Bind(slot);
		//shader.Bind();
		//shader.SetUniforms1i("u_Texture", slot);
		//shader.SetUniforms4f("u_Color", 0.2f, 0.3f, 0.4f, 1.0f);
		
		while (!glfwWindowShouldClose(window))
		{
			/// Render goes here.
			renderer.Clear();
			// Input manager
			int stateESC = glfwGetKey(window, GLFW_KEY_ESCAPE);
			if(stateESC == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, true);
			}
			texture.Bind(slot);
			// shader.Bind();
			//shader.SetUniforms1i("u_Texture", slot);
			
			renderer.Draw(vertexArray, indexBuffer, shader);
			
			glfwSwapBuffers(window);
			glfwPollEvents(); /// Evita que se quede bloqueada la pantalla.
			/// glfwWaitEvents(); Si solo quisieramos que la pantalla se actualizara cuando recibe un Input.
		}
	}
	glfwTerminate();
	return 0;
}

void const read3dObj(std::string filename) 
{
	// Lectura secuencial de ficheros.
	std::fstream file; // Input/Output file
	file.open(filename);
	
	if(!file)
	{
		LOG("Error reading file"+filename)
		exit(-3);
	}
	
	char readed;
	while (file.read(&readed, 1))
	{
		LOG(readed)
	}
	file.close();
	
	// Random read files
	file.open(filename);
	if (!file)
	{
		LOG("Error reading file" + filename)
			exit(-4);
	}
	file.seekg(1, std::ios::beg);
	LOG("random")
	while (file.read(&readed, 1))
	{
		LOG(readed)
	}
	file.close();
}

unsigned int CreateShader(const std::string& vertex_shader, const std::string& framentShader)
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

ShaderProgramSrc ParseShader(const std::string& filepath)
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

unsigned int CompileShader(GLenum type, std::string source)
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
