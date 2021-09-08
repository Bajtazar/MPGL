#version 330 core

out vec4 FragColor;

in vec4 fontColor;
in vec2 textureCoords;

uniform sampler2D tex;

void main() {
    FragColor = vec4(1.f, 1.f, 1.f, texture(tex, textureCoords).x) * fontColor;
}