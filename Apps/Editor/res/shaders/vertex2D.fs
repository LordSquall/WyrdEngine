/* Sprite 2D Fragment Shader */
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

uniform vec4 blendColor;

void main()
{
	color = v_Color * blendColor;
}