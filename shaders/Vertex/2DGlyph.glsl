#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 coords;
layout (location = 2) in vec4 color;

out vec4 fontColor;
out vec2 textureCoords;

void main() {
    gl_Position = vec4(position.xy, 0.f, 1.f);
    textureCoords = coords;
    fontColor = color;
}