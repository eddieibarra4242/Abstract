#version 330

in vec2 texCoord0;

uniform sampler2D R_filterImage;

uniform float R_texelStepX;
uniform float R_texelStepY;

vec3 COLOR_PALETTE[16];

vec3 getClosestColor(vec3 color)
{
    float dist = 255*255 + 255*255 + 255*255 + 1;
    vec3 closestColor = COLOR_PALETTE[0];

    for(int i = 0; i < 16; i++)
    {
        vec3 diff = (color * 255) - (COLOR_PALETTE[i] * 255);
        vec3 sqDiff = diff * diff;
        float d = sqDiff.r + sqDiff.g + sqDiff.b;

        if(d < dist)
        {
            dist = d;
            closestColor = COLOR_PALETTE[i];
        }
    }

    return closestColor;
}

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

    vec2 texelSampleTC0 = vec2(texCoord0.x - R_texelStepX, texCoord0.y               );
    vec2 texelSampleTC1 = vec2(texCoord0.x + R_texelStepX, texCoord0.y - R_texelStepY);
    vec2 texelSampleTC2 = vec2(texCoord0.x               , texCoord0.y - R_texelStepY);
    vec2 texelSampleTC3 = vec2(texCoord0.x - R_texelStepX, texCoord0.y - R_texelStepY);

    vec3 texelSample0 = texture2D(R_filterImage, texelSampleTC0).rgb;
    vec3 texelSample1 = texture2D(R_filterImage, texelSampleTC1).rgb;
    vec3 texelSample2 = texture2D(R_filterImage, texelSampleTC2).rgb;
    vec3 texelSample3 = texture2D(R_filterImage, texelSampleTC3).rgb;

    vec3 texelSampleCC0 = getClosestColor(texelSample0);
    vec3 texelSampleCC1 = getClosestColor(texelSample1);
    vec3 texelSampleCC2 = getClosestColor(texelSample2);
    vec3 texelSampleCC3 = getClosestColor(texelSample3);

    vec3 error0 = texelSample0 - texelSampleCC0;
    vec3 error1 = texelSample1 - texelSampleCC1;
    vec3 error2 = texelSample2 - texelSampleCC2;
    vec3 error3 = texelSample3 - texelSampleCC3;

    vec3 FSED0 = 7.0/16.0 * error0;
    vec3 FSED1 = 3.0/16.0 * error1;
    vec3 FSED2 = 5.0/16.0 * error2;
    vec3 FSED3 = 1.0/16.0 * error3;

    gl_FragColor = vec4(clamp(getClosestColor(texture2D(R_filterImage, texCoord0).rgb) + FSED0 + FSED1 + FSED2 + FSED3, 0.0, 1.0), 1);
}