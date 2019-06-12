#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_Position;
in vec2 v_UV;

uniform sampler2D s_Texture;

void main()
{
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	color = texture(s_Texture, v_UV);
}