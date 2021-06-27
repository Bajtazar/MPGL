#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

out vec4 color2;

void main()
{
    gl_Position = vec4(position.xy, 0.0, 1.0);
    color2 = color;
}
