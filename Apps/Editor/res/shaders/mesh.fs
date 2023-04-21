#version 330 core

layout(location = 0) out vec4 color;

in vec2 uv;

uniform sampler2D s_Texture;
uniform vec4 u_BlendColor;

void main()
{
	color = texture(s_Texture, uv) * u_BlendColor;
}