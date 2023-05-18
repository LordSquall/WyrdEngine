#version 330 core

layout(location = 0) out vec4 color;

in vec2 uv;

uniform vec4 u_EntityColor;

void main()
{
	color = u_EntityColor;
}