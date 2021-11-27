#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 RGB_COLOR;

out vec3 color;
out vec2 texCoord;
out vec3 normal;
out vec3 frag_position;


void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.f);
	color = RGB_COLOR;
}

// view nos da la vision de la camara, si lo quitamos, los objetos no se mueven.