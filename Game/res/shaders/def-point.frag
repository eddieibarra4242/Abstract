#version 330

uniform vec3 C_eyePos;
uniform float specIntensity;
uniform float specPower;

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};

struct PointLight
{
	BaseLight light;
	vec3 position;
	Attenuation atten;
	float range;
};

vec4 calcLight(BaseLight light, vec3 direction, vec3 normal, vec3 worldPos)
{
	float diffuseFactor = dot(normal, -direction);

	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specColor = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0)
	{
		diffuseColor = vec4(light.color, 1.0) * light.intensity * diffuseFactor;

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

vec4 calcPointLight(PointLight light, vec3 normal, vec3 worldPos)
{
	vec3 lightDirection = worldPos - light.position;
	float dist = length(lightDirection);

	//if(dist > light.range)
	//{
	//	return vec4(0,0,0,0);
	//}

	lightDirection = normalize(lightDirection);

	vec4 color = calcLight(light.light, lightDirection, normal, worldPos);

	float attenuation = light.atten.constant +
						light.atten.linear * dist + 
						light.atten.exponent * dist * dist + 0.0001;

	return color / attenuation;
}

in vec2 texCoord0;

uniform sampler2D R_gbufPos;
uniform sampler2D R_gbufNorm;
uniform sampler2D R_gbufAlbedo;
uniform sampler2D R_filterImage;

uniform PointLight R_pointLight;

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

	vec4 light = vec4(calcPointLight(R_pointLight, normal, worldPos).xyz, 1);
    float refr = light.r - 1.0;
    float refg = light.g - 1.0;
    float refb = light.b - 1.0;

    gl_FragColor = prevColor + color * light + vec4(max(0, refr), max(0, refg), max(0, refb), 1.0);
}