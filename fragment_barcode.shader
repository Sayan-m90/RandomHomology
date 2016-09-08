#version 120
#define PI 3.14159

varying vec4 dstColor;

void main() {
    gl_FragColor = vec4(dstColor.xyz, .8);
}