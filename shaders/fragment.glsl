#version 460 core

in vec2 v_screen_uv;

out vec4 frag_color;

uniform sampler2D t;
uniform vec2 fft_size;
uniform vec2 viewport_size;

vec2 sample_fft_tex(float a) {
	const float line_thickness = 40.0;
	const float channel_separation = 10.0;

	const float sample_width = 1.0 / fft_size.x;
	const float min_sample_uv = sample_width * 0.5;
	const float max_sample_uv = 1.0 - sample_width * 0.5;

	const float offset = (1.0 - v_screen_uv.x) * 0.24;

	const vec2 sampling_uv = vec2(mix(min_sample_uv, max_sample_uv, pow(v_screen_uv.x + offset, 5.0)), a);

	const vec2 pixel_pos = v_screen_uv * viewport_size;
	const float pixel_uv_width = 1.0 / viewport_size.x;
	const float half_thickness = float(line_thickness) / 2.0;

	float amp = texture(t, sampling_uv).r;

	float column_y = amp * viewport_size.y;

	return vec2(step(abs(column_y - pixel_pos.y), half_thickness), amp);
}

void main() {
	const vec4 left_color_low = vec4(0.5, 1.0, 0.0, 1.0);
	const vec4 left_color_high = vec4(1.0, 0.0, 0.5, 1.0);

	const vec4 right_color_low = vec4(0.5, 0.0, 1.0, 1.0);
	const vec4 right_color_high = vec4(0.0, 1.0, 0.5, 1.0);

	vec2 left_sample = sample_fft_tex(0.0);
	vec2 right_sample = sample_fft_tex(1.0);

	frag_color = mix(left_color_low, left_color_high, left_sample.y) * left_sample.x + mix(right_color_low, right_color_high, right_sample.y) * right_sample.x;
}
