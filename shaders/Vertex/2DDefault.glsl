#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 color2;

void main()
{
    gl_Position = vec4(position.xyz, 1.0);
    color2 = color;
}
