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

#define NR_POINT_LIGHTS 4

in vec3 Positions;
in vec3 Normal;
in vec2 TexCoord;
in vec3 color;

out vec4 Fragcolor;

float near = 0.3f;
float far = 100.f;

vec3 directional_light_calculations();
vec3 point_light_calculations(PointLight light);
vec3 spot_light_calculations();

vec3 light_ambient = vec3(0.9f);
vec3 light_diffuse = vec3(0.1f);
vec3 light_specular = vec3(0.01f);
uniform float material_shininess = 32;

uniform vec3 viewer_position;
uniform DirectionalLight dir_light;
uniform PointLight point_light[NR_POINT_LIGHTS];

void main() 
{
vec3 normalized_color = color;
	vec3 directional = directional_light_calculations();
	vec3 result =  vec3(1, 0, 1)+directional;
	if(normalized_color.x > 1.f) 
	{
		normalized_color.x = normalized_color.x/256;
		normalized_color.y = normalized_color.y/256;
		normalized_color.z = normalized_color.z/256;
	}
//	for(int i = 0; i<NR_POINT_LIGHTS; i++)
//	{
//		result += point_light_calculations(point_light[i]);
//	}
    Fragcolor = vec4(normalized_color, 1.f);
}

vec3 directional_light_calculations()
{
	vec3 viewer_direction = normalize(viewer_position - Positions);
	vec3 light_dir = normalize(-dir_light.direction);
	float diff = max(dot(Normal, light_dir), 0.0f);
	vec3 reflect_dir = reflect(-light_dir, vec3(1.f));
	float spec = pow(max(dot(viewer_direction, reflect_dir), 0.0f), material_shininess);
	vec3 ambient = light_ambient * color;
	vec3 diffuse = light_diffuse * diff * color;
	vec3 specular = light_specular * spec * color;

	return ambient + diffuse + specular;
}

vec3 point_light_calculations(PointLight light)
{
	vec3 viewer_direction = normalize(viewer_position - Positions);
	vec3 light_dir = normalize(light.position - Positions);
	float diff = max(dot(Normal, light_dir), 0.0f);
	vec3 reflect_dir = reflect(-light_dir, Normal);
	float spec = pow(max(dot(viewer_direction, reflect_dir), 0.0f), material_shininess);
	float distance = length(light.position-Positions);
	float attenuation = 1.0f/(light.k_constant + light.k_linear*distance + light.k_quadratic*(distance*distance));
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec;
	return ambient + diffuse + specular;
}
vec3 spot_light_calculations()
{
	return vec3(0.f);	
}