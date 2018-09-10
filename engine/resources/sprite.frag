#version 150

precision highp float;

uniform sampler2D u_texture;

in 	vec2 ex_TextureUV;

out vec4 gl_FragColor;

void main(void) {

	vec4 finalColor = texture2D( u_texture, ex_TextureUV );
    gl_FragColor = finalColor;
}