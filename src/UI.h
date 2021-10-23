#pragma once
#ifndef C__UI_H
#define C__UI_H

#include "vendor/imgui.h"
#include "vendor/imgui_impl_glfw.h"
#include "vendor/imgui_impl_opengl3.h"

class UI
{
	public:
	bool PositionWindow(float& x_, float& y_, float& z_); // true OK, false CANCEL
};

#endif // !C__UI_H