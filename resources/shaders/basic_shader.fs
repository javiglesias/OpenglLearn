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
	sampler2D diffuse_map;
	sampler2D specular_map;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

float near = 0.3f;
float far = 100.f;

vec3 directional_light_calculations(DirectionalLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction);
vec3 point_light_calculations(PointLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction);
vec3 spot_light_calculations(vec3 normal, vec3 frag_position, vec3 viewer_direction);
float LinearizeDepth(float depth);

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
// Obtnemos las coordenadas normalizadas del dispositivo.
	float depth = LinearizeDepth(frag_position.z);
	vec3 color = vec3(94.f,157.f,52.f);
	if(color.x > 1.f) 
	{
		color.x = color.x/256 * depth;
		color.y = color.y/256;
		color.z = color.z/256;
	}
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += point_light_calculations(point_light[i], normal, frag_position, viewer_direction);
	}
	FragColor = vec4(color, 1.0);
}

float LinearizeDepth(float depth) 
{
	float z = depth * 2 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

vec3 directional_light_calculations(DirectionalLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction)
{
	vec3 light_dir = normalize(-light.direction);
	float diff = max(dot(normal, light_dir), 0.0f);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(viewer_direction, reflect_dir), 0.0f), material.shininess);
	vec3 ambient = light.ambient * vec3(texture(material.diffuse_map, texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse_map, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular_map, texCoord));
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
	vec3 ambient = light.ambient * vec3(texture(material.diffuse_map, texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse_map, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular_map, texCoord));
	return ambient + diffuse + specular;
}
vec3 spot_light_calculations(vec3 normal, vec3 frag_position, vec3 viewer_direction)
{
	return vec3(1.0);	
}