#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aColor;

uniform mat4 transformation;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
   gl_Position = projection * view * model * vec4(aPos.x + gl_InstanceID, aPos.y, aPos.z, 1.f);
   TexCoords = aTexCoord;
}