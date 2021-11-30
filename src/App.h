#pragma once
#ifndef C__APP_HEADER
#define C__APP_HEADER
#include "vendor/imgui.h"
#include "vendor/imgui_impl_glfw.h"
#include "vendor/imgui_impl_opengl3.h"
#include "glad/glad.h"
// GLFW goes second
#include "GLFW/glfw3.h" 

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Shader.h"
#include "Model.h"
#include "Actor.h"
#include "Physics.h"

#include "../dependencies/stb_image/stb_image.h"

class App
{
public:
	App() {}
	int Run(int args, char** argv, unsigned int mode=0);
private:
	float position[3]{ 0.F, 0.F, 0.F };
	float rotate_axis[3]{ 1.F,1.F, 1.F };
	float rotation = 1.f;
	unsigned int models_count = 0;
	ImVec4 rgba_color = ImVec4(1.f, 0.f, 0.f, 0.f);
	float scale_vec[3]{ 1.F,1.F, 1.F };
	Physics* m_physics{};
	std::vector<std::string> models{};
	unsigned int triangles_drawed = 0;
	void GetModelsOnFolder();
};
#endif // !C__APP_HEADER
