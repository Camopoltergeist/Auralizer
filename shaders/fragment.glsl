#version 460 core

in vec2 v_screen_uv;

out vec4 frag_color;

uniform sampler2D t;
uniform vec2 fft_size;
uniform vec2 viewport_size;

void main() {
	const float line_thickness = 40.0;
	const float channel_separation = 10.0;
	const float lod_level = 0.0;
	const vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

	const float sample_width = 1.0 / fft_size.x;
	const float min_sample_uv = sample_width;
	const float max_sample_uv = 1.0 - sample_width;

	const vec2 sampling_uv = vec2(mix(min_sample_uv, max_sample_uv, v_screen_uv.x), 0.5);

	const vec2 pixel_pos = v_screen_uv * viewport_size;
	const float pixel_uv_width = 1.0 / viewport_size.x;
	const float half_thickness = float(line_thickness) / 2.0;

	float amp = textureLod(t, sampling_uv, lod_level).r;

	float column_y = amp * viewport_size.y;

	float s = step(abs(column_y - pixel_pos.y), half_thickness);

	frag_color = color * s;
}
