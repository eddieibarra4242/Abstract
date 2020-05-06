#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normals;

out vec2 texCoord0;
out vec3 normal0;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
    gl_Position = MVP * vec4(position, 1);
    texCoord0 = texCoord;
    normal0 = normalize((model * vec4(normals, 0)).xyz);
}