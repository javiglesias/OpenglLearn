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