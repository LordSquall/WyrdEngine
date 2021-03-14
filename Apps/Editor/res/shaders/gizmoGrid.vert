#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 viewProjection;
uniform mat4 model;
uniform mat4 scale;

out vec2 v_Position;
out vec4 v_Color;

void main()
{
	v_Position = a_Position;
	v_Color = a_Color;
	gl_Position =  viewProjection * model * scale * vec4(a_Position, 0.0, 1.0);	
}