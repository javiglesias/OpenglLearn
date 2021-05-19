#shader frament
#version 460 core
layout(location=0) out vec4 color;
uniform vec4 u_Color;
void main()
{
	color = (0.2f, 0.3f, 0.8f, 1.0f);
};
