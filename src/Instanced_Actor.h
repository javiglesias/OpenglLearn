#pragma once
#ifndef C__INSTANCED_ACTOR_H
#define C__INSTANCED_ACTOR_H

#include "Actor.h"

namespace Object {

    class Instanced_Actor : public Actor
    {
    public:
        Instanced_Actor(glm::mat4 _view, glm::mat4 _projection,
			glm::vec3 _camera_position, bool _visible);
        Instanced_Actor() {};
    };
}
#endif // !C__INSTANCED_ACTOR_H
