#include "Instanced_Actor.h"

using namespace Object;

Instanced_Actor::Instanced_Actor(glm::mat4 _view, glm::mat4 _projection,
	glm::vec3 _camera_position, bool _visible)
{
}

Instanced_Actor::Instanced_Actor(BASIC_SHAPES _basic_shape=BASIC_SHAPES::Cube)
{
	basic_shape = _basic_shape;
}
