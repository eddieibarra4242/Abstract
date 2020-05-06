#version 330

in vec2 texCoord0;

uniform vec3 R_ambientLight;
uniform sampler2D R_filterImage;

void main()
{
	vec4 color = texture2D(R_filterImage, texCoord0);

	if(color.a != 0)
	{
		color *= vec4(R_ambientLight, 1);
	}

	gl_FragColor = color;
}