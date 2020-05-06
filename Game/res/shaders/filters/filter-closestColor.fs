#version 330

in vec2 texCoord0;

uniform sampler2D R_filterImage;

vec3 COLOR_PALETTE[16];

void main()
{
    COLOR_PALETTE[ 0] = vec3(0.0, 0.0, 0.0);
    COLOR_PALETTE[ 1] = vec3(0.0, 0.0, 0.67);
    COLOR_PALETTE[ 2] = vec3(0.0, 0.67, 0.0);
    COLOR_PALETTE[ 3] = vec3(0.0, 0.67, 0.67);
    COLOR_PALETTE[ 4] = vec3(0.67, 0.0, 0.0);
    COLOR_PALETTE[ 5] = vec3(0.67, 0.0, 0.67);
    COLOR_PALETTE[ 6] = vec3(0.67, 0.33, 0.0);
    COLOR_PALETTE[ 7] = vec3(0.67, 0.67, 0.67);
    COLOR_PALETTE[ 8] = vec3(0.33, 0.33, 0.33);
    COLOR_PALETTE[ 9] = vec3(0.33, 0.33, 1.0);
    COLOR_PALETTE[10] = vec3(0.33, 1.0, 0.33);
    COLOR_PALETTE[11] = vec3(0.33, 1.0, 1.0);
    COLOR_PALETTE[12] = vec3(1.0, 0.33, 0.33);
    COLOR_PALETTE[13] = vec3(1.0, 0.33, 1.0);
    COLOR_PALETTE[14] = vec3(1.0, 1.0, 0.33);
    COLOR_PALETTE[15] = vec3(1.0, 1.0, 1.0);

    vec3 color = texture2D(R_filterImage, texCoord0).rgb;
    float dist = 255*255 + 255*255 + 255*255 + 1;
    vec3 finalColor = COLOR_PALETTE[0];

    for(int i = 0; i < 16; i++)
    {
        vec3 diff = (color * 255) - (COLOR_PALETTE[i] * 255);
        vec3 sqDiff = diff * diff;
        float d = sqDiff.r + sqDiff.g + sqDiff.b;

        if(d < dist)
        {
            dist = d;
            finalColor = COLOR_PALETTE[i];
        }
    }

    gl_FragColor = vec4(finalColor, 1);
}