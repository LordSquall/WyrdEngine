/* Sprite 2D Vertex Shader */
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;

uniform mat4 viewProjection;
uniform mat4 model;

out vec2 v_Position;
out vec2 v_UV;

void main()
{
	v_Position = a_Position;
	v_UV = a_UV;
	gl_Position = viewProjection * model * vec4(a_Position, 0.0, 1.0);	
}