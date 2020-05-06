#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_Position;
in vec2 v_UV;

uniform sampler2D s_Texture;
uniform vec3 blendColor;

void main()
{
	color = texture(s_Texture, v_UV) * vec4(blendColor, 1.0f);
}