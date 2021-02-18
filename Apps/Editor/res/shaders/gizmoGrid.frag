#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_Position;

uniform vec4 blendColor;

void main()
{
	color = blendColor;
}