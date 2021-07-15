#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoords_;

out vec2 textureCoords;

void main()
{
    gl_Position = vec4(position.xy, 0.0, 1.0);
    textureCoords = textureCoords_;
}
