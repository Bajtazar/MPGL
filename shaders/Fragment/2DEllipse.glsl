#version 330 core

uniform vec4 color;
uniform vec2 shift;
uniform mat2 transform;

void main() {
    vec2 local = transform * (gl_FragCoord.xy - shift);
    if (step(distance(local, vec2(0.5)), 0.5) == 0.)
        discard;
    gl_FragColor = color;
}
