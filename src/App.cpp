#include "App.h"
#include <iostream>
#include <fstream>
#include "Utils.h"
#include <wtypes.h>

#define GUI

// FREE FUNCTIONS
void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
	glViewport(0, 1, width, height);
	glm::ortho(0.f, (float)width, 0.f, (float)height, 0.3f, 10.f);
}
void mouse_movement_callback(GLFWwindow* window, double x_position, double y_position)
{
	if (!bShowGUICursor)
	{
		float_t x_offset = x_position - fLastXPosition;
		float_t y_offset = fLastYPosition - y_position;
		float_t senseo = 0.1f;
		fLastXPosition = x_position;
		fLastYPosition = y_position;
		x_offset *= senseo;
		y_offset *= senseo;
		fYaw += x_offset;
		fPitch += y_offset;
		// CONSTRAINTS
		if (fPitch > 89.0f) fPitch = 89.0f;
		if (fPitch < -89.0f) fPitch = -89.0f;
		glm::vec3 camera_direction;
		camera_direction.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
		camera_direction.y = sin(glm::radians(fPitch));
		camera_direction.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
		vCameraForward = glm::normalize(camera_direction);
	}
}
void mouse_scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	fFieldOfView -= y_offset;
	if (fFieldOfView < 1.f) fFieldOfView = 1.f;
	if (fFieldOfView > 100.f) fFieldOfView = 100.f;
}
void process_input(GLFWwindow* m_window)
{
	// CAMERA MOVEMENT
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		vCameraPosition += fCameraSpeed * vCameraForward;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		vCameraPosition += glm::normalize(glm::cross(
			vCameraUp, vCameraForward)) * fCameraSpeed;
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		vCameraPosition -= fCameraSpeed * vCameraForward;
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		vCameraPosition -= glm::normalize(glm::cross(
			vCameraUp, vCameraForward)) * fCameraSpeed;
	}
	// FPS old school
	// vCameraPosition.y = y_constant;

	if (glfwGetKey(m_window, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		fShininess += 1;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_END) == GLFW_PRESS)
	{
		fShininess -= 1;
	}
	if (glfwGetKey(m_window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
	{
		fScale += 0.01f;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
	{
		fScale -= 0.01f;
	}

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		bShowGUICursor = false;
	}
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		bShowGUICursor = true;
	}
}
std::string ExePath() {
	char pwd[256]{};
	return std::string(pwd);
}
// Dedicated GPU for laptops with 2 https://stackoverflow.com/questions/16823372/forcing-machine-to-use-dedicated-graphics-card
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

void RefreshShaders()
{
	vLoadedShaders.clear();
	vLoadedShaders.push_back(Shader("basic_shader","resources/shaders/basic_shader.vert",
		"resources/shaders/basic_shader.frag"));
	vLoadedShaders.push_back(Shader("basic_shape_shader", "resources / shaders / basic_shape_shader.vert",
		"resources/shaders/basic_shape_shader.frag"));
	vLoadedShaders.push_back(Shader("light_shader", "resources / shaders / light_shader.vert",
		"resources/shaders/light_shader.frag"));
	vLoadedShaders.push_back(Shader("material_shader", "resources/shaders/material_shader.vert",
		"resources/shaders/material_shader.frag"));
	vLoadedShaders.push_back(Shader("grass_shader", "resources/shaders/grass_shader.vert",
		"resources/shaders/grass_shader.frag"));
}
int App::Run(int args, char** argv, unsigned int mode) // mode Debug:0 Retail:1 
{
	// INICIALIZAMOS GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
		GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* m_window = glfwCreateWindow(iScreenWidth, iScreenHeight,
		"LearnOpenGL", nullptr, nullptr);
	if (m_window == nullptr)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error loading GLAD.";
		return -1;
	}

	// INICIALIZAMOS IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& input_output = ImGui::GetIO();
	(void)input_output;
	// INICIALIZAMOS EL ESTULO DE IMGUI
	ImGui::StyleColorsDark();
	// ESTABLECEMOS LA PATAFORMA Y RENDER
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// CALLBACKS
	framebuffer_size_callback(m_window, iScreenWidth, iScreenHeight);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_movement_callback);
	glfwSetScrollCallback(m_window, mouse_scroll_callback);

	glEnable(GL_BLEND);

	//Render prep
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1.f, 0xff);

	// CREATE SHADER INSTANCE AND PROGRAM
	vLoadedShaders.push_back(Shader("basic_shader", "resources/shaders/basic_shader.vert",
		"resources/shaders/basic_shader.frag"));
	vLoadedShaders.push_back(Shader("basic_shape_shader", "resources / shaders / basic_shape_shader.vert",
		"resources/shaders/basic_shape_shader.frag"));
	vLoadedShaders.push_back(Shader("light_shader", "resources / shaders / light_shader.vert",
		"resources/shaders/light_shader.frag"));
	vLoadedShaders.push_back(Shader("material_shader", "resources/shaders/material_shader.vert",
		"resources/shaders/material_shader.frag"));
	vLoadedShaders.push_back(Shader("grass_shader", "resources/shaders/grass_shader.vert",
		"resources/shaders/grass_shader.frag"));

	// Vector cross product gives us a orthogonal vector;
	vCameraRight = glm::vec3(glm::normalize(glm::cross(vUp, vCameraForward)));
	// this cross gives us the last direction orthogonal
	vCameraUp = glm::vec3(glm::cross(vCameraDirection, vCameraRight));

	mLookAt = glm::mat4(
		glm::vec4(vCameraRight, 0.f),
		glm::vec4(vCameraUp, 0.f),
		glm::vec4(vCameraForward, 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f));
	mLookAt *= glm::mat4(
		glm::vec4(1.f, 0.f, 0.f, vCameraPosition.x),
		glm::vec4(0.f, 1.f, 0.f, vCameraPosition.y),
		glm::vec4(0.f, 0.f, 1.f, vCameraPosition.z),
		glm::vec4(0.f, 0.f, 0.f, 1.f));

	// fixed timestep
	fCurrentFrame = glfwGetTime();
	char sTempShape[64];
	Actor oNewActor(Model("resources/models/BasicShapes/Monkey.obj"), vLoadedShaders[0],
		glm::mat4(1.f), mView, mProjection,
		vCameraPosition, _(Monkey));
	oNewActor.setPosition(position[0], position[1], position[2]);
	vModelsLoaded.push_back(oNewActor);
	++iCurrentModelsDrawn;

	//Render loop
	while (!glfwWindowShouldClose(m_window))
	{
		fNewFrame = glfwGetTime();
		fDeltaTime = fNewFrame - fCurrentFrame;
		fCurrentFrame = fNewFrame;
		fTimeAccumulated += fDeltaTime;
		fTimeAccumulated_physics += fDeltaTime;
		// Frame fixed a 60
		if (fTimeAccumulated > fFrameCap)
		{
			process_input(m_window);
			if (bShowGUICursor)
			{
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			
			mProjection = glm::perspective(glm::radians(fFieldOfView),
				(float)iScreenWidth / iScreenHeight, fZNear, fZFar);

			mView = glm::lookAt(vCameraPosition, vCameraPosition + vCameraForward,
				vCameraUp); // camera up direction
			iFrameNumber++;
			
			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glBindVertexArray(0);

			// Render Loop
			for (uint8_t i = 0; i < iCurrentModelsDrawn; i++)
			{
				Actor* oActor = &vModelsLoaded[i];
				if (vModelsLoaded[i].visible)
				{
					//Set the Shader properties for the model
					unsigned int color_location = glGetUniformLocation(oActor->getShader().id, "RGB_COLOR");
					glUniform3fv(color_location, 1, glm::value_ptr(oActor->getShader().rgba_color));
					oActor->getModelLoaded().Draw(oActor->getShader(), oActor->getModel(),
						mView, mProjection, vCameraPosition,
						vLightPosition);
				}
			}

			// IMGUI
#ifdef GUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::LabelText("Fps", std::to_string(1 / fTimeAccumulated).c_str());
			ImGui::LabelText("Frame", std::to_string(iFrameNumber).c_str());
			ImGui::LabelText("Camera X", std::to_string(vCameraPosition.x).c_str());
			ImGui::SameLine();
			ImGui::LabelText("Camera Y", std::to_string(vCameraPosition.y).c_str());
			ImGui::SameLine();
			ImGui::LabelText("Camera Z", std::to_string(vCameraPosition.z).c_str());

			ImGui::Begin("World outliner");
			// TODO: make selectable the items of the outliner
			for (auto it = 0; it < vModelsLoaded.size(); it++)
			{
				bool to_delete = false;
				auto model_to_draw = vModelsLoaded.at(it);
				static int current_outliner = 0;
				if (ImGui::SmallButton("X"))
				{
					to_delete = true;
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("O"))
				{
					RefreshShaders();
				}
				ImGui::SameLine();
				ImGui::LabelText("",model_to_draw.getName().c_str());
				if (to_delete)
				{
					vModelsLoaded.erase(vModelsLoaded.begin() + it);
					--iCurrentModelsDrawn;
				}
			}
			ImGui::End();

			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Quit"))
					{
						glfwSetWindowShouldClose(m_window, true);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
			ImGui::Begin("Tools");
			if (ImGui::ArrowButton("left", ImGuiDir_Left))
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				sPolygonMode = "Wireframe";
			}
			ImGui::SameLine();
			ImGui::Text("%s", sPolygonMode);
			ImGui::SameLine();
			if (ImGui::ArrowButton("right", ImGuiDir_Right))
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				sPolygonMode = "Solid";
			}
			if (ImGui::ArrowButton("Culling ON", ImGuiDir_Left))
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				//glFrontFace(GL_CCW); //This shows the model inside-out backfaces CW
			}
			ImGui::SameLine();
			ImGui::Text("Culling");
			ImGui::SameLine();
			if (ImGui::ArrowButton("Culling OFF", ImGuiDir_Right))
			{
				glDisable(GL_CULL_FACE);
			}
			if (ImGui::ArrowButton("Stencil 0x00", ImGuiDir_Left))
			{
				glEnable(GL_STENCIL_BUFFER_BIT);
				glStencilMask(0x00);
				glStencilFunc(GL_EQUAL, 0.5f, 0xff);
			}
			ImGui::SameLine();
			ImGui::Text("Stencil");
			ImGui::SameLine();
			if (ImGui::ArrowButton("Disable Stencil", ImGuiDir_Right))
			{
				glDisable(GL_STENCIL_BUFFER_BIT);
			}
			if (ImGui::ArrowButton("Disable Depth", ImGuiDir_Left))
			{
				glDisable(GL_DEPTH_TEST);
			}
			ImGui::SameLine();
			ImGui::Text("Depth");
			ImGui::SameLine();
			if (ImGui::ArrowButton("Enable Depth", ImGuiDir_Right))
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
			}
			if (ImGui::ArrowButton("Cull Back", ImGuiDir_Left))
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
			ImGui::SameLine();
			ImGui::Text("Cull Front/back");
			ImGui::SameLine();
			if (ImGui::ArrowButton("Cull Front", ImGuiDir_Right))
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
			}
			if (ImGui::Button("Toggle cursor"))
			{
				bShowGUICursor = !bShowGUICursor;
			}
			if (ImGui::ArrowButton("Material Shine +", ImGuiDir_Up))
			{
				fShininess += 1.f;
			}
			ImGui::SameLine();
			ImGui::Text("Shine");
			ImGui::SameLine();
			if (ImGui::ArrowButton("Material Shine -", ImGuiDir_Down))
			{
				fShininess -= 1.f;
			}
			if (ImGui::ArrowButton("Scale +", ImGuiDir_Up))
			{
				fScale += 0.01f;
			}
			ImGui::SameLine();
			ImGui::Text("scale");
			ImGui::SameLine();
			if (ImGui::ArrowButton("Scale -", ImGuiDir_Down))
			{
				fScale -= 0.01f;
			}
			if (ImGui::Button("Frame cap 30"))
			{
				fFrameCap = FRAMECAP30;
			}
			if (ImGui::Button("Frame cap 60"))
			{
				fFrameCap = FRAMECAP60;
			}
			if (ImGui::Button("step by step"))
			{
				fFrameCap = 1;
			}
			if (ImGui::MenuItem("Directional light", "", bDirectionalLightEnabled))
			{
				bDirectionalLightEnabled = !bDirectionalLightEnabled;
			}
			if (ImGui::MenuItem("Spotlight", "", bPointLightEnabled))
			{
				bPointLightEnabled = !bPointLightEnabled;
			}
			if (ImGui::MenuItem("Point light", "", bSpotLightEnabled))
			{
				bSpotLightEnabled = !bSpotLightEnabled;
			}
			ImGui::End();
			ImGui::Begin("Create Model");
			ImGui::Text("Set the position and rotation to spawn:");
			ImGui::Separator();
			static float position[3];
			ImGui::InputFloat3("position", position);
			static float rotation[1];
			ImGui::InputFloat("rotation angles degrees", rotation);
			static int radio_button;
			
			if (ImGui::RadioButton("Cube", &radio_button, 0))
			{
				sTempActorName = _(Cube);
			}
			else if (ImGui::RadioButton("Cone", &radio_button, 1))
			{
				sTempActorName = _(Cone);
			}
			else if (ImGui::RadioButton("Cylinder", &radio_button, 2))
			{
				sTempActorName = _(Cylinder);
			}
			else if (ImGui::RadioButton("Plane", &radio_button, 3))
			{
				sTempActorName = _(Plane);
			}
			else if (ImGui::RadioButton("Torus", &radio_button, 4))
			{
				sTempActorName = _(Torus);
			}
			else if (ImGui::RadioButton("LightBulb", &radio_button, 5))
			{
				sTempActorName = _(Lightbulb);
			}
			else if (ImGui::RadioButton("Monkey", &radio_button, 6))
			{
				sTempActorName = _(Monkey);
			}
			else if (ImGui::RadioButton("Sphere", &radio_button, 7))
			{
				sTempActorName = _(Sphere);

			}
			static int current_shader_idx = 0;
			Shader current_shader_selected = vLoadedShaders[current_shader_idx];
			const char* current_shader_name = current_shader_selected.name;
			if (ImGui::BeginCombo("Shader to apply", current_shader_name, NULL))
			{
				for (int i = 0; i < vLoadedShaders.size(); i++)
				{
					auto shader = vLoadedShaders[i];
					const bool is_selected = (current_shader_idx == i);
					if (ImGui::Selectable(shader.name, is_selected))
					{
						current_shader_idx = i;
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::TreeNode("Color pick shader"))
			{
				ImGui::ColorEdit3("rgb color", (float*)&rgba_color, NULL);
				ImGui::TreePop();
			}
			if (ImGui::Button("OK"))
			{
				// TODO: rotation of the model.
				glm::mat4 basic_cube_model(1.f);
				basic_cube_model = glm::translate(basic_cube_model,
					glm::vec3(position[0], position[1], position[2]));
				glm::rotate(basic_cube_model, glm::radians(rotation[0]),
					glm::vec3(position[0], position[1], position[2]));
				if (vModelsAlreadyLoaded.size() > 0)
				{
					for (uint8_t i = 0; i < vModelsAlreadyLoaded.size(); i++)
					{
						if(strcmp(vModelsAlreadyLoaded[i].hash, sTempActorName.c_str()) == 0)
						{
							oTempModelToCreate = &vModelsAlreadyLoaded[i].data;
							break;
						}
					}
				}

				if(!oTempModelToCreate)
				{
					char sModelName[64];
					sprintf(sModelName, "resources/models/BasicShapes/%s.obj", sTempActorName.c_str());
					oTempModelToCreate = new Model(sModelName);
					vModelsAlreadyLoaded.push_back(tHashItem<Model>((char*)sTempActorName.c_str(), *oTempModelToCreate));
				}
				current_shader_selected.rgba_color = glm::vec4(rgba_color.x, rgba_color.y, rgba_color.z, rgba_color.w);

				Actor oNewActor(*oTempModelToCreate, current_shader_selected,
					basic_cube_model, mView, mProjection,
					vCameraPosition, sTempActorName);
				oNewActor.setPosition(position[0], position[1], position[2]);
				vModelsLoaded.push_back(oNewActor);
				++iCurrentModelsDrawn;
			}
			ImGui::End();

			// RENDER THE DATA FOR THE GUI
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

			// poll the events and call the callback functions.
			glfwPollEvents();
			// swap the Color buffer
			glfwSwapBuffers(m_window);
			fTimeAccumulated = 0;
		}
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
    return 0;
}

