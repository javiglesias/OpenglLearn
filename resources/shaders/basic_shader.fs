#version 330 core

struct DirectionalLight 
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float k_constant;
	float k_linear;
	float k_quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight 
{
	float cutoff;
	float outer_cutoff;
};

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

vec3 directional_light_calculations(DirectionalLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction);
vec3 point_light_calculations(PointLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction);
vec3 spot_light_calculations(vec3 normal, vec3 frag_position, vec3 viewer_direction);

#define NR_POINT_LIGHTS 4

out vec4 FragColor;

uniform vec3 viewer_position;

uniform DirectionalLight dir_light;
uniform PointLight point_light[NR_POINT_LIGHTS];
uniform Material material;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 frag_position;

void main()
{
	vec3 viewer_direction = normalize(viewer_position - frag_position);
	vec3 result = directional_light_calculations(dir_light, normal, frag_position, viewer_direction);
	FragColor = vec4(result, 1.0f);
}
vec3 directional_light_calculations(DirectionalLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction)
{
	vec3 light_dir = normalize(-light.direction);
	float diff = max(dot(normal, light_dir), 0.0f);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(viewer_direction, reflect_dir), 0.0f), material.shininess);
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));
	return ambient + diffuse + specular;
}

vec3 point_light_calculations(PointLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction)
{
	vec3 light_dir = normalize(light.position - frag_position);
	float diff = max(dot(normal, light_dir), 0.0f);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(viewer_direction, reflect_dir), 0.0f), material.shininess);
	float distance = length(light.position-frag_position);
	float attenuation = 1.0f/(light.k_constant + light.k_linear*distance + light.k_quadratic*(distance*distance));
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));
	return ambient + diffuse + specular;
}
vec3 spot_light_calculations(vec3 normal, vec3 frag_position, vec3 viewer_direction)
{
	return vec3(1.0);	
}