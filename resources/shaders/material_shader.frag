#version 330 core

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

in vec2 texCoord;
in vec3 normal;
in vec3 frag_position;

out vec4 FragColor;

uniform Material material;
uniform vec3 viewer_position;

void main()
{
	vec3 viewer_direction = normalize(viewer_position - frag_position);
	vec3 result = vec3(texture(material.texture_diffuse1, texCoord));
	FragColor = texture(material.texture_diffuse1, texCoord);
}