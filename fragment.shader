#version 120
#define PI 3.14159

varying vec4 dstColor;
uniform vec4 ourColor;

void main() {
    // set multicolor traversal
    float t = dstColor.x;
    gl_FragColor = vec4(.5 + .5 * cos(2.*PI*(.8 * (t-.1) + 0.0)),
                        .5 + .5 * cos(2.*PI*(.8 * (t-.1) + 0.66)),
                        .5 + .5 * cos(2.*PI*(.8 * (t-.1) + 0.33)),
                        1.0);
    //gl_FragColor = vec4(t);
}