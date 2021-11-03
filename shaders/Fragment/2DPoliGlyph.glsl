#version 330 core

in vec4 color;
in vec2 textureCoords;

uniform sampler2D tex;

void main() {
	gl_FragColor = vec4(1.f, 1.f, 1.f, texture(tex, textureCoords).x) * color;
}
