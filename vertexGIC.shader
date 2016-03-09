#version 120
attribute vec4 position;
attribute vec4 next_position;
attribute vec4 color;
attribute vec3 normal;
varying vec4 dstColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float t;
void main() {
    gl_PointSize = 4.0;
    vec4 smooth_pos = t*next_position + (1.0-t)*position;
    gl_Position = projection * view * model * (smooth_pos);
    dstColor = color;
}