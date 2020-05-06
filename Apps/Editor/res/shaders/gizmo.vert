#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;

uniform mat4 viewProjection;
uniform vec2 positionOffset;

out vec2 v_Position;
out vec2 v_UV;

void main()
{
	v_Position = a_Position;
	v_UV = a_UV;
	gl_Position = viewProjection * vec4(a_Position + positionOffset, 0.0, 1.0);	
}