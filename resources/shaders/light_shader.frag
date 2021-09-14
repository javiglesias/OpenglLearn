#version 330 core

out vec4 frag_color;

in vec2 TexCoords;

uniform sampler2D texture_bulb;

void main()
{
   vec4 texColor = texture(texture_bulb, TexCoords);
	if(texColor.a < 0.1)
		discard;;
	//228,112,37 warm
	frag_color = vec4(1.f);
}