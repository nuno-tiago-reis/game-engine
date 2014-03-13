#version 330 core
#pragma optionNV(unroll all)

#define LIGHT_COUNT 5

#define SPOT_LIGHT 1
#define POSITIONAL_LIGHT 2
#define DIRECTIONAL_LIGHT 3

#define SPOTLIGHT_OUTER_ANGLE 0.97

/* Input Attributes (Same as the Real-Wood Vertex Shader) */
in vec4 Fragment_Position;

in vec3 Fragment_Normal;

in vec4 Fragment_WoodColor;
in vec4 Fragment_ModelPosition;

in vec4 Fragment_Ambient;
in vec4 Fragment_Diffuse;
in vec4 Fragment_Specular;
in float Fragment_SpecularConstant;

in vec3 LightDirection[LIGHT_COUNT];
in vec3 HalfwayVector[LIGHT_COUNT];

/* Uniforms */
uniform sampler3D Noise;
uniform float NoiseScale;

uniform mat4 ModelMatrix;

layout(std140) uniform SharedMatrices {

	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

struct LightSource {

	vec4 Position;
	vec4 Direction;

	vec4 Color;

	float CutOff;

	float AmbientIntensity;
	float DiffuseIntensity;
	float SpecularIntensity;

	float ConstantAttenuation;
	float LinearAttenuation;
	float ExponentialAttenuation;

	int LightType;
};

layout(std140) uniform SharedLightSources {

	LightSource LightSources[LIGHT_COUNT];
};

/* Output Attributes (Fragment Color) */
out vec4 Fragment_Color;

/* Shared Variables between Light Sources */
vec3 Normal;
vec4 WoodColor;

vec4 positionalLight(int i) {
	
	/* Light LightDistance / Direction */
	float LightDistance = length(ViewMatrix * LightSources[i].Position - Fragment_Position);

	/* Light Intensity */
	float LightIntensity = 1.0 / (LightSources[i].ConstantAttenuation + LightSources[i].LinearAttenuation * LightDistance + LightSources[i].ExponentialAttenuation * LightDistance * LightDistance);

	/* Ambient Component */
	vec4 AmbientColor = WoodColor * LightSources[i].Color * LightSources[i].AmbientIntensity;
	vec4 DiffuseColor = vec4(0, 0, 0, 1);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 1);

	/* Diffuse Component */
	float DiffuseFactor = max(dot(Normal, normalize(LightDirection[i])), 0.0);

	if (DiffuseFactor > 0.0) {

		DiffuseColor = WoodColor * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

		/* Specular Component */
		float SpecularAngle = max(dot(Normal, normalize(HalfwayVector[i])), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, Fragment_SpecularConstant);                
		if(SpecularFactor > 0.0)
			SpecularColor = WoodColor * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
	}

	/* Final Calculation */
	return AmbientColor + (DiffuseColor + SpecularColor) * LightIntensity;
}

vec4 directionalLight(int i) {

	/* Ambient Component */
	vec4 AmbientColor = Fragment_Ambient * LightSources[i].Color * LightSources[i].AmbientIntensity;
	vec4 DiffuseColor = vec4(0, 0, 0, 1);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 1);

	/* Diffuse Component */
	float DiffuseFactor = max(dot(Normal, -normalize(LightDirection[i])), 0.0);

	if (DiffuseFactor > 0) {

		DiffuseColor = Fragment_Diffuse * WoodColor * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

		/* Specular Component */
		float SpecularAngle = max(dot(Normal, normalize(HalfwayVector[i])), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, Fragment_SpecularConstant);                               
		if(SpecularFactor > 0.0)
			SpecularColor = Fragment_Specular * WoodColor * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
	}

	/* Final Calculation */
	return AmbientColor + DiffuseColor + SpecularColor;
}

vec4 spotLight(int i) {

	/* Light LightDistance / Direction */
	vec3 LightToVertex = vec3(ViewMatrix * LightSources[i].Position - Fragment_Position);
	float LightDistance = length(LightToVertex);  

	LightToVertex = normalize(LightToVertex);

	/* Light Intensity */
	float LightIntensity = 1.0 / (LightSources[i].ConstantAttenuation + LightSources[i].LinearAttenuation * LightDistance + LightSources[i].ExponentialAttenuation * LightDistance * LightDistance);

	/* Ambient Component */
	vec4 AmbientColor = Fragment_Ambient * LightSources[i].Color * LightSources[i].AmbientIntensity;
	vec4 DiffuseColor = vec4(0, 0, 0, 0);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 0);
	
	/* Diffuse Component */
	float DiffuseFactor = max(dot(Normal, LightToVertex), 0.0);

	if (DiffuseFactor > 0) {

		/* SpotLight Circular effect fading around the edges */
		float CosineDifference = SPOTLIGHT_OUTER_ANGLE - LightSources[i].CutOff ;

		float SpotEffect = clamp((dot(normalize(LightDirection[i]), -LightToVertex) - LightSources[i].CutOff) / CosineDifference, 0.0, 1.0);

		DiffuseColor = Fragment_Diffuse * WoodColor * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor * SpotEffect;

		/* Specular Component */			                
		float SpecularAngle = max(dot(Fragment_Normal, normalize(HalfwayVector[i])), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, Fragment_SpecularConstant);                               
		if(SpecularFactor > 0.0)
			SpecularColor = Fragment_Specular * WoodColor * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor * SpotEffect;
	}

	/* Final Calculation */
	return AmbientColor + (DiffuseColor + SpecularColor) * LightIntensity;
}
void main() {

	/* Fragment Normal */
	Normal = normalize(Fragment_Normal);

	/* Fragment WoodColor */

		/* Wood Color Calculations */
		vec4 DarkShade=vec4(0.4, 0.2, 0.07, 1.0);
		vec4 LightShade=vec4(0.6, 0.3, 0.1, 1.0);
		float RepetitionRate=25;
		float RepetitionScale=9.42;
		float RepetitionSharpness=50.0;

		mat4 RotationMatrix;
		RotationMatrix[0]=vec4(0.75,-0.21651,0.625,0);
		RotationMatrix[1]=vec4(0.43301,0.875,-0.21651,0);
		RotationMatrix[2]=vec4(-0.5,0.43301,0.75,0);
		RotationMatrix[3]=vec4(0,0,0,1);
	
		vec3 RotatedPosition =  (RotationMatrix * Fragment_ModelPosition * NoiseScale).xyz;
		float RotatedNoise = 2.0 * texture(Noise, RotatedPosition).r - 1.0;
		float r = fract(RepetitionRate * (Fragment_ModelPosition.y * NoiseScale) + RepetitionScale * RotatedNoise);
		float invMax = pow(RepetitionSharpness, RepetitionSharpness / (RepetitionSharpness - 1.0))/(RepetitionSharpness - 1.0);
		float ring = invMax * (r - pow(r, RepetitionSharpness));
		float lerp = ring + RotatedNoise;
	
		WoodColor = mix(DarkShade, LightShade, lerp);

	//WoodColor = Fragment_WoodColor;

	/* Fragment Color */
	Fragment_Color = vec4(0);
	
	/* Light computing */
	for(int i=0; i<LIGHT_COUNT; i++) {

		switch(LightSources[i].LightType) { 

			case POSITIONAL_LIGHT:	Fragment_Color += positionalLight(i);
									break;

			case DIRECTIONAL_LIGHT:	Fragment_Color += directionalLight(i);
									break;

			case SPOT_LIGHT:		Fragment_Color += spotLight(i);
									break;
		}
	}
}