#version 330 core

in vec2 coords;

uniform vec4 color;
uniform sampler2D tex;

void main() {
    gl_FragColor = vec4(1.f, 1.f, 1.f, texture(tex, coords).x) * color;
}
