#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

uniform mat4 transformation;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float CHUNK;
uniform float SCALE;
uniform vec3 RGB_COLOR;

out vec3 color;
out vec2 texCoord;
out vec3 normal;
out vec3 frag_position;

void main()
{
	float index_ID = int(gl_InstanceID/CHUNK);
	float local_instance_ID = gl_InstanceID;
	if(index_ID > 0)
	{
		local_instance_ID = CHUNK * index_ID - gl_InstanceID;
	}
	vec3 pos_instance  = vec3(aPos.x + local_instance_ID , aPos.y, aPos.z + index_ID) * SCALE;
	
	gl_Position = projection * view * model * vec4(pos_instance, 1.f);
	texCoord = aTexCoord;
	frag_position = aPos;
	normal = aNormal;
	color = RGB_COLOR;// vec3(94.f,157.f,52.f);
}