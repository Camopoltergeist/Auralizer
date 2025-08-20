#version 460 core

in vec2 v_screen_uv;

out vec4 frag_color;

uniform sampler2D t;
uniform vec2 fft_size;
uniform vec2 viewport_size;

float logWarp(float u, float xmin, float curvature) {
    float base = pow(1.0 / xmin, curvature);
    return xmin * pow(base, u);
}

float inverse_lerp(float start, float end, float value) {
    return (value - start) / (end - start);
}

float get_sampling_uv_x_from_index(float bar_index, float bar_count) {
    return logWarp(bar_index / bar_count, 2.0 / fft_size.x, 1.0);
}

float get_relative_position_on_bar(float bar_index, float bar_count, float screen_uv_x) {
    float left_uv_x = bar_index / bar_count;
    float right_uv_x = (bar_index + 1.0) / bar_count;

    return inverse_lerp(left_uv_x, right_uv_x, screen_uv_x);
}

void main() {
    const vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    const float bar_count = 100.0;
    const float bar_width = 1.0 / bar_count;

    float bar_index = floor(bar_count * v_screen_uv.x);
    float relative_pos = get_relative_position_on_bar(bar_index, bar_count, v_screen_uv.x);

    float left_sampling_uv_x = get_sampling_uv_x_from_index(bar_index, bar_count);
    float right_sampling_uv_x = get_sampling_uv_x_from_index(bar_index + 1.0, bar_count);

    vec2 left_sampling_uv = vec2(left_sampling_uv_x, 0.5);
    vec2 right_sampling_uv = vec2(right_sampling_uv_x, 0.5);

    float left_amp = textureLod(t, left_sampling_uv, 0.0).r;
    float right_amp = textureLod(t, right_sampling_uv, 0.0).r;

    float amp = mix(left_amp, right_amp, relative_pos);

    float s = step(v_screen_uv.y, amp);

    frag_color = color * s;
}
