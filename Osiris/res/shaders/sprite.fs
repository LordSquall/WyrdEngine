#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_Position;
void main()
{
	color = vec4(vec3(v_Position, 0.0) * 0.5 + 0.5, 1.0);
}