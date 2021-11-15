#pragma once
#ifndef C__2DAPP_H
#define C__2DAPP_H
#include <cstdio>
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define FRAMECAP60 0.01666666666666667
#define FRAMECAP30 0.03333333333333333

class App2D
{
public:
	App2D(){}
	int Run(int args, char** argv);

	float m_new_frame = 0.f;
	float m_delta_time = 0.f;
	float m_current_frame = 0.f;
	float m_accumulated_time = 0.f;
	float m_accumulated_time_physics = 0.f;
	float m_frame_cap = FRAMECAP60;
	float m_frame_cap_physics = FRAMECAP30;
	unsigned int m_frame_number = 0;

};
#endif
