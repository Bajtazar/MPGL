#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 coords;
layout (location = 2) in vec4 textureColor;

out vec4 color;
out vec2 textureCoords;

void main() {
    gl_Position = vec4(position.xy, 0.0, 1.0);
    textureCoords = coords;
    color = textureColor;
}
