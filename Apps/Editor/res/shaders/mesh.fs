#version 330 core

layout(location = 0) out vec4 color;

in vec2 uv;

uniform sampler2D u_Texture;
uniform vec2 u_Tiling;
uniform vec4 u_BlendColor;

void main()
{
	color = texture(u_Texture, (uv * u_Tiling)) * u_BlendColor;
}