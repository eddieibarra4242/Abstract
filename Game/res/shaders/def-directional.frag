#version 330

uniform vec3 C_eyePos;
uniform float specIntensity;
uniform float specPower;

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

vec4 calcLight(BaseLight light, vec3 direction, vec3 normal, vec3 worldPos)
{
	float diffuseFactor = dot(normal, -direction);

	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specColor = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0)
	{
		diffuseColor = vec4(light.color, 1.0) * light.intensity * diffuseFactor;

		vec3 directionToEye = normalize(C_eyePos - worldPos);
		//vec3 ref = normalize(reflect(direction, normal));
		vec3 halfDirection = normalize(directionToEye - direction);

		float specFactor = dot(halfDirection, normal);
		//float specFactor = dot(directionToEye, ref);
		specFactor = pow(specFactor, specPower);

		if(specFactor > 0)
		{
			specColor = vec4(light.color, 1.0) * specIntensity * specFactor;
		}
	}

	return diffuseColor + specColor;
}

vec4 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 worldPos)
{
	return calcLight(light.base, -light.direction, normal, worldPos);
}

in vec2 texCoord0;

uniform sampler2D R_gbufPos;
uniform sampler2D R_gbufNorm;
uniform sampler2D R_gbufAlbedo;
uniform sampler2D R_filterImage;

uniform DirectionalLight R_directionalLight;

void main()
{
    vec4 prevColor = texture2D(R_filterImage, texCoord0);

	if(prevColor.a == 0)
	{
		gl_FragColor = prevColor;
		return;
	}

    vec3 worldPos = texture2D(R_gbufPos, texCoord0).rgb;
    vec3 normal = texture2D(R_gbufNorm, texCoord0).rgb;
    vec4 color = texture2D(R_gbufAlbedo, texCoord0);

	vec4 light = vec4(calcDirectionalLight(R_directionalLight, normal, worldPos).xyz, 1);
    float refr = light.r - 1.0;
    float refg = light.g - 1.0;
    float refb = light.b - 1.0;

    gl_FragColor = prevColor + color * light + vec4(max(0, refr), max(0, refg), max(0, refb), 1.0);
}