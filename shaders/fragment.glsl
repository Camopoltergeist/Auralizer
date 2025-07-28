#version 460 core

in vec2 v_uv;

out vec4 frag_color;

uniform sampler2D t;

void main() {
	frag_color = texture(t, v_uv);
}