/* Text Vertex Shader */
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;
out vec2 v_UV;

uniform mat4 viewProjection;

void main()
{
	gl_Position = viewProjection * vec4(a_Position, 0.0, 1.0);	
	v_UV = a_UV;
}