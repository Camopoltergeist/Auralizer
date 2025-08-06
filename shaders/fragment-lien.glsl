#version 460 core

in vec2 v_uv;

out vec4 frag_color;

uniform sampler2D t;
uniform vec2 fft_size;
uniform vec2 viewport_size;

void main() {
	const float lod_level = 3.0;
	const float thickness = 10.0;

	const vec2 pixel_pos = v_uv * viewport_size;

	float pixel_uv_width = 1.0 / viewport_size.x;

	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

	float mult = 0.0;

	for(int i = 0; i < int(thickness) / 2; i++) {
		vec2 left_uv = vec2(v_uv.x - float(i) * pixel_uv_width, v_uv.y);
		vec2 right_uv = vec2(v_uv.x + float(i) * pixel_uv_width, v_uv.y);

		float left_amp = textureLod(t, left_uv, lod_level).r;
		vec2 left_pos = vec2(left_uv.x, left_amp) * viewport_size;
		float left_dist = distance(left_pos, pixel_pos);

		mult = max(step(left_dist, thickness), mult);

		float right_amp = textureLod(t, right_uv, lod_level).r;
		vec2 right_pos = vec2(right_uv.x, right_amp) * viewport_size;
		float right_dist = distance(right_pos, pixel_pos);

		mult = max(step(right_dist, thickness), mult);
	}

	frag_color = color * mult;
}