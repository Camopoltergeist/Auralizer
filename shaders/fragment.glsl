#version 460 core

in vec2 v_uv;

out vec4 frag_color;

uniform sampler2D t;
uniform vec2 screen_size;

void main() {
	const vec2 pixel_size = 1.0 / screen_size;
	const float line_height = pixel_size.y * 10.0;

	float mag = (texture(t, v_uv).r + 1.0) / 2.0;
	float dist = abs(v_uv.y - mag);
	float mult = step(dist, line_height);

	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

	frag_color = color * mult;
}