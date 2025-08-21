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

float sample_index(float bar_index, float bar_count, float uv_y) {
    float uv_x = get_sampling_uv_x_from_index(bar_index, bar_count);

    return textureLod(t, vec2(uv_x, uv_y), 0.0).r;
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

float distance_to_infinite_line(vec2 point_a, vec2 point_b, vec2 target) {
    return abs((point_b.y - point_a.y) * target.x - (point_b.x - point_a.x) * target.y + point_b.x * point_a.y - point_b.y * point_a.x) / sqrt(pow(point_b.y - point_a.y, 2.0) + pow(point_b.x - point_a.x, 2.0));
}

float distance_to_line(vec2 point_a, vec2 point_b, vec2 target) {
    vec2 line_ab = point_b - point_a;
    vec2 line_at = target - point_a;

    float t = dot(line_at, line_ab) / dot(line_ab, line_ab);

    t = clamp(t, 0.0, 1.0);

    vec2 closest_point = point_a + t * line_ab;

    return distance(target, closest_point);
}

float distance_to_segment_by_index(float segment_index, float segment_count, float channel) {
    const vec2 pixel_pos = v_screen_uv * viewport_size;

    float left_x = segment_index / segment_count;
    float right_x = (segment_index + 1.0) / segment_count;

    float left_y = sample_index(segment_index, segment_count, channel);
    float right_y = sample_index(segment_index + 1.0, segment_count, channel);

    return distance_to_line(vec2(left_x, left_y) * viewport_size, vec2(right_x, right_y) * viewport_size, pixel_pos);
}

float a(float segment_index, float segment_count, float thickness, float channel) {
    float current_d = distance_to_segment_by_index(segment_index, segment_count, channel);
    float left_d = distance_to_segment_by_index(segment_index - 1.0, segment_count, channel);
    float right_d = distance_to_segment_by_index(segment_index + 1.0, segment_count, channel);

    float d = min(min(current_d, left_d), right_d);

    const float half_thickness = thickness / 2.0;

    return 1.0 - smoothstep(half_thickness - 0.5, half_thickness + 0.5, d);
}

void main() {
    const vec4 left_color_low = vec4(0.5, 1.0, 0.0, 1.0);
    const vec4 left_color_high = vec4(1.0, 0.0, 0.5, 1.0);

    const vec4 right_color_low = vec4(0.5, 0.0, 1.0, 1.0);
    const vec4 right_color_high = vec4(0.0, 1.0, 0.5, 1.0);

    const vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    const float bar_count = 100.0;
    const float thickness = 20.0;
    const float half_thickness = thickness / 2.0;

    float bar_index = floor(bar_count * v_screen_uv.x);

    float left = a(bar_index, bar_count, thickness, 0.0);
    float right = a(bar_index, bar_count, thickness, 1.0);

    frag_color = mix(left_color_low, left_color_high, v_screen_uv.y) * left + mix(right_color_low, right_color_high, v_screen_uv.y) * right;
}
