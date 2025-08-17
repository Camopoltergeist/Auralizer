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

void main() {
    const vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    const float bar_count = 1000.0;
    const float bar_width = 1.0 / bar_count;

    float bar_index = floor(bar_count * v_screen_uv.x);
    float sampling_uv_x = logWarp(bar_index / bar_count, 2.0 / fft_size.x, 1.0);

    float bar_center = (bar_index + 0.5) * bar_width;

    vec2 sampling_uv = vec2(sampling_uv_x, 0.5);

    float amp = textureLod(t, sampling_uv, 0.0).r;

    float s = step(v_screen_uv.y, amp);

    frag_color = color * s;
}
