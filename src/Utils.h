#ifndef C__RENDER
#define C__RENDER

#include "Actor.h"
#include "vendor/imgui.h"

#define _(x) #x
#define FRAMECAP60 0.016667
#define FRAMECAP30 0.033333
#define CHUNK 1000
#define DEBUG 1 // 0 retail

template<class T>
struct tHashItem {
	const char* hash;
	T data;
	tHashItem(char* _hash, T _data)
	{
		hash = _hash;
		data = _data;
	}
};

char aBasicShapes[8][64] =
	{
		"Cube",
		"Cone",
		"Cylinder",
		"Plane",
		"Sphere",
		"Torus",
		"LightBulb",
		"Monkey"
	};
enum GUI_COMMANDS
{
	Text,
	SameLine
};
struct GUI_command
{
	GUI_COMMANDS command;
	std::string value;

	GUI_command(GUI_COMMANDS _command, std::string _value)
	{
		command = _command;
		value = _value;
	}
};
struct model_loaded
{
	Model model_load;
	Shader shader;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 camera_position;
	std::string name;
	bool visible = true;

	model_loaded(Model _model_load, Shader _shader,
		glm::mat4 _model, glm::mat4 _view, glm::mat4 _projection,
		glm::vec3 _camera_position, std::string _name = "model_",
		bool _visible = true)
	{
		model_load = _model_load;
		shader = _shader;
		model = _model;
		view = _view;
		projection = _projection;
		camera_position = _camera_position;
		name = _name;
		visible = _visible;
	}
};

std::vector<tHashItem<Model>>	vModelsAlreadyLoaded;
std::vector<Actor>				vModelsLoaded;
std::vector<Actor>				vModelsToSave;
std::vector<Actor>				vStaticWorld;
std::vector<Actor>				vDynamicWorld;
std::vector<Actor>				vLightsLoaded;
std::vector<Shader>				vLoadedShaders;

ImVec4							vLightColor(255.f, 255.f, 255.f, 1.f);

bool							bVAO_MODE = true;
bool							bCustomModel = false;
bool							bShowGUICursor = true;
bool							bDirectionalLightEnabled = false;
bool							bPointLightEnabled = false;
bool							bSpotLightEnabled = false;

glm::vec3						vCamera(0.f, 0.f, 0.3f);
glm::vec3						vCameraTarget(0.f, 0.f, 0.f);
glm::vec3						vCameraDirection(glm::normalize(vCamera - vCameraTarget));
glm::vec3						vCameraPosition(0.f);
glm::vec3						vCameraForward(0.f, 0.f, 1.f);
glm::vec3						vCameraUp(0.f, 1.f, 0.f);
glm::vec3						vCameraRight(1.f, 0.f, 0.f);
glm::vec3						vUp(0.f, 1.f, 0.f);
glm::vec4						vLightAmbient(1.f, 1.f, 1.f, 1.f);
glm::vec4						vLightDiffuse(1.f, 1.f, 1.f, 1.f);
glm::vec4						vLightSpecular(1.f, 1.f, 1.f, 1.f);
glm::vec3						vLightPosition(1.f);
glm::vec3						vLightDirectional(0.f, 0.f, 1.f);
glm::vec3						vPointLightsPosition[4];
glm::vec3						vCoral(1.f, .5f, .31f);

glm::mat4						mProjection(1.f);
glm::mat4						mView(1.f);
glm::mat4						mLookAt(1.f);

uint16_t						iScreenWidth = 1024;
uint16_t						iScreenHeight = 720;
uint16_t						iFrameNumber = 0;
uint16_t						iCurrentModelsDrawn = 0;

float_t							fScale = 1.f;
float_t							fZNear = 0.3f;
float_t							fZFar = 10.f;
float_t							fFieldOfView = 90.f;
float_t							fYaw;
float_t							fPitch;
float_t							fLastXPosition;
float_t							fLastYPosition;
float_t							fLight_k_constant = 1.0;
float_t							fLight_k_linear = 0.09f;
float_t							fLight_k_quadratic = 0.032f;
float_t							fLight_cutoff = 12.5f;
float_t							fLight_outer_cutoff = 17.2f;
float_t							fShininess = 32.f;
float_t							fCameraSpeed = 0.01f;
float_t							fDeltaTime = 0.f;
float_t							fNewFrame = 0.f;
float_t							fTimeAccumulated = 0.f;
float_t							fTimeAccumulated_physics = 0.f;
float_t							fFrameCap = FRAMECAP60;
float_t							fCurrentFrame = 0.f;
float_t							fMatShineMode = 32.f;
float_t							fScaleMode = 32.f;

const char*						sPolygonMode{"Solid"};
const char*						sCullingMode{ "ON" };
const char*						sStencilMode{ "ON" };
const char*						sDepthMode{ "ON" };
const char*						sCullMode{ "Back" };

#endif