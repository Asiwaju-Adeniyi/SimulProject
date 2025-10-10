#version 330 core

layout (location = 0) in vec2 aPos;
uniform vec3 uColor;

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    gl_PointSize = 8.0;
    vertexColor = uColor;
}
