#version 330 core

uniform sampler2D fbo_texture;
uniform sampler2D fbo_texture2;
in vec2 f_texcoord;

out vec4 out_Color;

void main(void) {
	out_Color = texture2D(fbo_texture, f_texcoord)*0.57;
	out_Color += texture2D(fbo_texture2, f_texcoord)*0.43;
}