#version 330 core
struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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

out vec4 FragColor;

uniform vec3 viewer_position;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform Light light;
uniform Material material;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 frag_position;

void main()
{
	// AMBIENT LIGHTING
	float ambient_strength = 0.5;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse_map, texCoord));
  	// TEXTURIZE
	//mix(texture(texture2, texCoord),texture(texture1, texCoord), 0.2)
	
	// DIFFUSE LIGHTING
	vec3 normal_normalized = normalize(normal);
	vec3 light_direction = normalize(light.position - frag_position); // Light vector direction
	float diffuse_factor = max(dot(normal_normalized, light_direction), 0.0); //  see the observator and the normals of the material
	vec3 diffuse_light = light.diffuse * diffuse_factor * vec3(texture(material.diffuse_map, texCoord));

	// SPECULAR LIGHTING
	float specular_strength = 0.5;
	vec3 viewer_direction = normalize(viewer_position - frag_position);
	vec3 reflect_direction = reflect(-light_direction, normal);
	float specular_factor = pow(max(dot(viewer_direction, reflect_direction), 0.0), material.shininess);
	vec3 specular_light = specular_factor * vec3(texture(material.specular_map, texCoord)) * light.specular;
	// OBJECT
	vec3 result = ambient + diffuse_light + specular_light;
	FragColor = vec4(result, 1.0) * mix(texture(texture2, texCoord),texture(texture1, texCoord), 0.2);
}