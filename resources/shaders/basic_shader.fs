#version 330 core
out vec4 FragColor;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 viewer_position;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 frag_position;

void main()
{
	// AMBIENT LIGHTING
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength  * light_color;
  	// TEXTURIZE
	//mix(texture(texture2, texCoord),texture(texture1, texCoord), 0.2)
	
	// DIFFUSE LIGHTING
	vec3 normal_normalized = normalize(normal);
	vec3 light_direction = normalize(light_position - frag_position); // Light vector direction
	float diffuse_factor = max(dot(normal_normalized, light_direction), 0.0);
	vec3 diffuse_light = diffuse_factor * light_color;

	// SPECULAR LIGHTING
	float specular_strength = 0.5;
	vec3 viewer_direction = normalize(viewer_position - frag_position);
	vec3 reflect_direction = reflect(-light_direction, normal);
	float specular_factor = pow(max(dot(viewer_direction, reflect_direction), 0.0), 32);
	vec3 specular_light = specular_strength * specular_factor * light_color;
	// OBJECT
	vec3 result = ambient + diffuse_light + specular_light;
	FragColor = vec4(result, 1.0) * mix(texture(texture2, texCoord),texture(texture1, texCoord), 0.2);
}