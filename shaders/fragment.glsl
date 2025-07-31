#version 460 core

in vec2 v_uv;

out vec4 frag_color;

uniform sampler2D t;

void main() {
	const float thickness = 0.027;

	float mag = (texture(t, v_uv).r + 1.0) / 2.0;
	float dist = abs(v_uv.y - mag);
	float mult = step(dist, thickness);

	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

	frag_color = color * mult;
}