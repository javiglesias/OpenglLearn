#version 330 core
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
}

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
}

uniform Material material

uniform Light light;

void main()
{
	// AMBIENT LIGHTING
	float ambient_strength = 0.5;
	vec3 ambient = light.ambient * material.ambient;
  	// TEXTURIZE
	//mix(texture(texture2, texCoord),texture(texture1, texCoord), 0.2)
	
	// DIFFUSE LIGHTING
	vec3 normal_normalized = normalize(normal);
	vec3 light_direction = normalize(light_position - frag_position); // Light vector direction
	float diffuse_factor = max(dot(normal_normalized, light_direction), 0.0);
	vec3 diffuse_light = material.diffuse * light.diffuse;

	// SPECULAR LIGHTING
	float specular_strength = 0.5;
	vec3 viewer_direction = normalize(viewer_position - frag_position);
	vec3 reflect_direction = reflect(-light_direction, normal);
	float specular_factor = pow(max(dot(viewer_direction, reflect_direction), 0.0), material.shininess);
	vec3 specular_light = (material.specular * specular_factor) * light.specular;
	// OBJECT
	vec3 result = ambient + diffuse_light + specular_light;
	FragColor = vec4(result, 1.0) * mix(texture(texture2, texCoord),texture(texture1, texCoord), 0.2);
}