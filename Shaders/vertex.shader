#version 120
attribute vec4 position;
attribute vec4 color;
attribute vec3 normal;
varying vec4 dstColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_PointSize = 15.0;
	gl_Position = projection * view * model * (position.xyzw);
    dstColor = color;
}