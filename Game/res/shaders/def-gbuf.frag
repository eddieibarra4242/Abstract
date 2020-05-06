#version 330

layout (location = 0) out vec4 posColor;
layout (location = 1) out vec4 normColor;
layout (location = 2) out vec4 tanColor;
layout (location = 3) out vec4 albedoColor;

in vec3 worldPos0;
in vec2 texCoord0;
in vec3 normal0;
in vec3 tangent0;

uniform sampler2D diffuse;

void main()
{
	posColor = vec4(worldPos0, 1);
    normColor = vec4(normal0, 1);
    tanColor = vec4(tangent0, 1);
    albedoColor = texture2D(diffuse, texCoord0);
}