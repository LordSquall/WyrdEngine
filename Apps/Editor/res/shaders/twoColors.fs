#version 330 core

layout(location = 0) out vec4 color;

in vec2 uv;

uniform vec4 u_BaseColor;
uniform vec4 u_BlendColor;

void main()
{
	color = u_BlendColor * u_BaseColor;
}