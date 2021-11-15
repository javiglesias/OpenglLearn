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

#include "../dependencies/stb_image/stb_image.h"

#include <queue>

#define _(x) #x
#define FRAMECAP60 0.01666666666666667
#define FRAMECAP30 0.03333333333333333
#define CHUNK 1000
#define DEBUG 1


class App
{
public:
	App() {}
	int Run(int args, char** argv, unsigned int mode=0);
private:
	std::vector<std::string> models{};
	void GetModelsOnFolder();
};
#endif // !C__APP_HEADER
