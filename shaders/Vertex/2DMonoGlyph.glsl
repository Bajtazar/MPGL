#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoords;

out vec2 coords;

void main() {
    coords = textureCoords;
    gl_Position = vec4(position.xy, 0.f, 1.f);
}
