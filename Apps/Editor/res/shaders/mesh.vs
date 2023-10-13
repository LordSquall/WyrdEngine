#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_UV;

out vec2 uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	uv = vec2(a_UV.x, 1.0-a_UV.y);
	gl_Position = u_projection * u_view * u_model * vec4(a_Position, 1.0);	
}