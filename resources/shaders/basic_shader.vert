#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float CHUNK;
uniform float SCALE;
uniform vec3 RGB_COLOR;
uniform vec3 instance_positions[10]; // Max instances of each object


out vec3 color;
out vec2 texCoord;
out vec3 normal;
out vec3 frag_position;

void main()
{	
	vec3 instance_position[10];
	instance_position[0] = vec3(0);
	vec4 current_position = vec4(aPos.x + gl_InstanceID, aPos.y, aPos.z, 1.0); // position where draw the current instance
	gl_Position = projection * view * model * current_position;
	texCoord = aTexCoord;
	frag_position = aPos;
	normal = aNormal;
	color =  RGB_COLOR;
}