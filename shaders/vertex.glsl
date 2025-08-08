#version 460 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};

out vec2 v_screen_uv;

void main() {
	gl_Position = vec4(pos, 0.0, 1.0);
	v_screen_uv = uv;
}
