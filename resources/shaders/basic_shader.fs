#version 330 core
struct Light
{
	vec3 position; // No Es necesario cuando trabajamos con luz direccional.
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float k_constant;
	float k_linear;
	float k_quadratic;

	float cutoff;
	float outer_cutoff;
};

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

};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	sampler2D diffuse_map;
	sampler2D specular_map;
	vec3 specular;
	float shininess;
};

#define NR_POINT_LIGHTS 4

out vec4 FragColor;

uniform vec3 viewer_position;

uniform DirectionalLight dir_light;
uniform PointLight point_light[NR_POINT_LIGHTS];

uniform Light light;
uniform Material material;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 frag_position;
vec3 directional_light_calculations(DirectionalLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction);
vec3 point_light_calculations(PointLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction);
vec3 spot_light_calculations(SpotLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction);
void main()
{
	// AMBIENT LIGHTING
	float ambient_strength = 0.5;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse_map, texCoord));
  	vec3 light_dir = normalize(-light.direction);
	
	// DIFFUSE LIGHTING
	vec3 normal_normalized = normalize(normal);
	// vec3 light_direction = normalize(light.position - frag_position); // Light vector direction
	float diffuse_factor = max(dot(normal_normalized, light_dir), 0.0); //  see the observator and the normals of the material
	vec3 diffuse_light = light.diffuse * diffuse_factor; //* vec3(texture(material.diffuse_map, texCoord));

	// SPECULAR LIGHTING
	// TODO ARREGLAR LA SPECULAR, PORQUE ESTA MAL
	float specular_strength = 0.5;
	vec3 viewer_direction = normalize(viewer_position - frag_position);
	vec3 reflect_direction = reflect(-light_dir, normal);
	float specular_factor = pow(max(dot(viewer_direction, reflect_direction), 0.0), material.shininess);
	vec3 specular_light = specular_strength * light.specular * vec3(texture(material.specular_map, texCoord));

	//SPOTLIGHT
	float theta = dot(light_dir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outer_cutoff;
	float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

	// OBJECT
	float distance = length(light.position - frag_position);
	float attenuation = (1.0/(light.k_constant + light.k_linear*distance + light.k_quadratic*pow(distance, 2)));
	ambient *= attenuation;
		
	diffuse_light *= intensity;
	specular_light *= intensity;
		
	vec3 result = ambient + diffuse_light + specular_light;
	FragColor = vec4(result, 1.0) * texture(material.diffuse_map, texCoord);
}
vec3 directional_light_calculations(DirectionalLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction)
{
	vec3 light_dir = normalize(-light.position);
	float diff = max(dot(normal, light_dit), 0.0f);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float specular = pow(max(dot(viewer_direction, reflect_dir), 0.0f), material.shininess);
	vec3 ambien = light.ambient * vec3(texture(material.diffuse, texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
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
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
}
vec3 spot_light_calculations(SpotLight light, vec3 normal, vec3 frag_position, vec3 viewer_direction)
{
	
}