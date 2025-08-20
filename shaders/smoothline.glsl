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

float sample_point(vec2 screen_uv, float bar_count) {
    float bar_index = floor(bar_count * screen_uv.x);
    float relative_pos = get_relative_position_on_bar(bar_index, bar_count, screen_uv.x);

    float left_sampling_uv_x = get_sampling_uv_x_from_index(bar_index, bar_count);
    float right_sampling_uv_x = get_sampling_uv_x_from_index(bar_index + 1.0, bar_count);

    vec2 left_sampling_uv = vec2(left_sampling_uv_x, 0.5);
    vec2 right_sampling_uv = vec2(right_sampling_uv_x, 0.5);

    float left_amp = textureLod(t, left_sampling_uv, 0.0).r;
    float right_amp = textureLod(t, right_sampling_uv, 0.0).r;

    return mix(left_amp, right_amp, relative_pos);
}

float distance_to_line(vec2 point_a, vec2 point_b, vec2 target) {
    return abs((point_b.y - point_a.y) * target.x - (point_b.x - point_a.x) * target.y + point_b.x * point_a.y - point_b.y * point_a.x) / sqrt(pow(point_b.y - point_a.y, 2.0) + pow(point_b.x - point_a.x, 2.0));
}

void main() {
    const vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    const float bar_count = 100.0;
    const float thickness = 10.0;

    const float pixel_uv_width = 1.0 / viewport_size.x;
    const vec2 pixel_pos = v_screen_uv * viewport_size;

    float inside_mult = 0.0;

    for(int i = 0; i < int(thickness) / 2; i++) {
        vec2 left_uv = vec2(v_screen_uv.x - float(i) * pixel_uv_width, 0.5);
        float left_amp = sample_point(left_uv, bar_count);
        vec2 left_pos = vec2(left_uv.x, left_amp) * viewport_size;
        float left_dist = distance(left_pos, pixel_pos);

        inside_mult = max(step(left_dist, thickness), inside_mult);

        vec2 right_uv = vec2(v_screen_uv.x + float(i) * pixel_uv_width, 0.5);
        float right_amp = sample_point(right_uv, bar_count);
        vec2 right_pos = vec2(right_uv.x, right_amp) * viewport_size;
        float right_dist = distance(right_pos, pixel_pos);

        inside_mult = max(step(right_dist, thickness), inside_mult);
    }

    frag_color = color * inside_mult;
}
