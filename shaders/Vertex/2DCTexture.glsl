#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoords_;
layout (location = 2) in vec4 color;

out vec4 color2;
out vec2 textureCoords;

void main()
{
    gl_Position = vec4(position.xy, 0.0, 1.0);
    textureCoords = textureCoords_;
    color2 = color;
}
