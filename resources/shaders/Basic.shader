#shader vertex
#version 330 core

layout(location=0) in vec4 position;
layout(location = 1) in vec2 textCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = position;
	v_TexCoord = textCoord;
};

#shader fragment
#version 330 core

layout(location=0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	/*u_Color = vec4(0.8, 0.8, 0.8, 1.0);
	vec4 textColor = texture(u_Texture, v_TexCoord);*/
	color = u_Color;
};