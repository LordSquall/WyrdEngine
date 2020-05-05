#version 330 core

layout(location = 0) in vec2 a_Position;

uniform mat4 viewProjection;
uniform vec2 positionOffset;

out vec2 v_Position;

void main()
{
	v_Position = a_Position;
	gl_Position = viewProjection * vec4(a_Position + positionOffset, 0.0, 1.0);	
}