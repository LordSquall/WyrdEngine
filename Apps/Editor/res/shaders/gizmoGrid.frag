#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_Position;

uniform vec3 blendColor;

void main()
{
	color = vec4(blendColor, 1.0f);
}