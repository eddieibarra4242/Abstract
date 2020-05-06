#version 330

in vec2 texCoord0;

uniform sampler2D R_filterImage;

void main()
{
    gl_FragColor = texture2D(R_filterImage, texCoord0);
}