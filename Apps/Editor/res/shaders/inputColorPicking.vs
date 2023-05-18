#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_UV;

out vec2 uv;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	uv = a_UV;
	gl_Position = u_projection * u_view * vec4(a_Position, 1.0);	
}