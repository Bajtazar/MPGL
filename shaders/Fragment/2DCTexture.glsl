#version 330 core

in vec4 color;
in vec2 textureCoords;

uniform sampler2D tex;

void main() {
	gl_FragColor = texture(tex, textureCoords) * color;
}
