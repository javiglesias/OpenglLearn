#pragma once
#ifndef C__INSTANCED_ACTOR_H
#define C__INSTANCED_ACTOR_H

#include "Actor.h"

namespace Object {
	enum BASIC_SHAPES
	{
		Cube,
		Cone,
		Cylinder,
		Plane,
		Sphere,
		Torus,
		LightBulb,
		Monkey
	};

    class Instanced_Actor : public Actor
    {
    public:
        Instanced_Actor(glm::mat4 _view, glm::mat4 _projection,
			glm::vec3 _camera_position, bool _visible);
		Instanced_Actor(BASIC_SHAPES);

        BASIC_SHAPES basic_shape;
    };
}
#endif // !C__INSTANCED_ACTOR_H
