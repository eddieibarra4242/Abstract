#version 330

in vec2 texCoord0;
in vec3 normal0;

uniform sampler2D diffuse;

const vec3 lightDir = normalize(vec3(1, -1, 1));

void main()
{
    gl_FragColor = texture2D(diffuse, texCoord0) * clamp(dot(-lightDir, normal0), 0.2, 1);
}