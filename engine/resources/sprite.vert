#version 150
in  vec2 in_Position;
in  vec2 in_TextureUV;

uniform mat4 u_mvp;

out vec2 ex_TextureUV;

void main(void) {
    gl_Position = u_mvp * vec4(in_Position.x, in_Position.y, 0.0, 1.0);
 
	ex_TextureUV = in_TextureUV;
}