#version 330 core

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 frag_position;

out vec4 FragColor;

uniform Material material;

void main()
{
	vec3 result = vec3(texture(material.texture_diffuse1, texCoord));
	FragColor = vec4(result, 1.0f);
}