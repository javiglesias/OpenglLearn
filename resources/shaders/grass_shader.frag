#version 330 core

out vec4 frag_color;

in vec2 Texcoord;

uniform sampler2D texture_grass;

void main()
{
	vec4 texColor = texture(texture_grass, Texcoord);
	if(texColor.a < 0.1)
		discard;
	frag_color = texColor;
}