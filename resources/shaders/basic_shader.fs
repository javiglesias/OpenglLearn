#version 330 core
out vec4 FragColor;

uniform float green;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 light_color;

in vec3 color;
in vec2 texCoord;

void main()
{
   FragColor = light_color * mix(texture(texture2, texCoord),texture(texture1, texCoord), 0.2);
}