#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out vec3 worldPos0;
out vec2 texCoord0;
out vec3 normal0;
out vec3 tangent0;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
	gl_Position = MVP * vec4(position, 1);
    worldPos0 = (model * vec4(position, 1.0)).xyz;
    texCoord0 = texCoord;
    normal0 = normalize((model * vec4(normal, 0.0)).xyz);

    vec3 t = normalize((model * vec4(tangent, 0.0)).xyz);
    t = normalize(t - dot(t, normal0) * normal0);

    tangent0 = t;
}