/* Text Fragment Shader */
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_UV;

uniform sampler2D s_Texture;
uniform vec4 blendColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(s_Texture, v_UV).r);
	color = blendColor * sampled;
}