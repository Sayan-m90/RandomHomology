#version 120
attribute vec4 position;
attribute vec4 next_position;
attribute vec4 color;
attribute vec3 normal;
varying vec4 dstColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_PointSize = 0.0;
    vec4 smooth_pos = position;
    gl_Position = projection * view * model * (smooth_pos.xyzw);
    dstColor = color;
}