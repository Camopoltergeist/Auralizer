#version 460 core

in vec2 v_uv;

out vec4 frag_color;

uniform sampler2D t;
uniform vec2 fft_size;
uniform vec2 viewport_size;

void main() {
	const float line_thickness = 40.0;
	const float channel_separation = 10.0;
	const float lod_level = 0.0;

	float pixel_uv_width = 1.0 / viewport_size.x;
	float half_thickness = float(line_thickness) / 2.0;

	vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 left_color = vec4(0.0, 1.0, 0.0, 1.0);
	vec4 right_color = vec4(0.0, 0.0, 1.0, 1.0);

	float a = pow(v_uv.x, 2.0) + 0.5 / fft_size.x;

	float amp = textureLod(t, vec2(a, v_uv.y), lod_level).r;
	float sep = pow(amp * 2.0, 1.5);

	float column_y = amp * viewport_size.y;
	vec2 pixel_pos = v_uv * viewport_size;
	float s = step(abs(column_y - pixel_pos.y), half_thickness);

	// vec2 right_uv = vec2(v_uv.x + sep * channel_separation * pixel_uv_width, v_uv.y);
	// float off_screen_mult_right = step(right_uv.x, 1.0);
	// float right = textureLod(t, right_uv, lod_level).r * viewport_size.y;
	// vec2 right_pixel_pos = right_uv * viewport_size;
	// float right_s = step(abs(right - right_pixel_pos.y), half_thickness) * off_screen_mult_right;

	// vec2 left_uv = vec2(v_uv.x - sep * channel_separation * pixel_uv_width, v_uv.y);
	// float off_screen_mult_left = step(0.0, left_uv.x);
	// float left = textureLod(t, left_uv, lod_level).r * viewport_size.y;
	// vec2 left_pixel_pos = left_uv * viewport_size;
	// float left_s = step(abs(left - left_pixel_pos.y), half_thickness) * off_screen_mult_left;

	// frag_color = color * s + right_color * right_s + left_color * left_s;
	frag_color = (color + left_color + right_color) * s;
}