#version 330 core
#pragma optionNV(unroll all)

#define LIGHT_COUNT 5

#define SPOT_LIGHT 1
#define POSITIONAL_LIGHT 2
#define DIRECTIONAL_LIGHT 3

#define SPOTLIGHT_OUTER_ANGLE 0.97

/* Input Attributes (Same as the Blinn-Phong Vertex Shader) */
in vec4 Fragment_Position;

in vec3 Fragment_Normal;

in vec4 Fragment_Ambient;
in vec4 Fragment_Diffuse;
in vec4 Fragment_Specular;
in float Fragment_SpecularConstant;

in vec3 LightDirection[LIGHT_COUNT];
in vec3 HalfwayVector[LIGHT_COUNT];

/* Uniforms */
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

vec3 Normal;

vec4 positionalLight(int i) {
	
	/* Light LightDistance / Direction */
	float LightDistance = length(ViewMatrix * LightSources[i].Position - Fragment_Position);

	/* Light Intensity */
	float LightIntensity = 1.0 / (LightSources[i].ConstantAttenuation + LightSources[i].LinearAttenuation * LightDistance + LightSources[i].ExponentialAttenuation * LightDistance * LightDistance);

	/* Ambient Component */
	vec4 AmbientColor = Fragment_Ambient * LightSources[i].Color * LightSources[i].AmbientIntensity;
	vec4 DiffuseColor = vec4(0, 0, 0, 1);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 1);

	/* Diffuse Component */
	float DiffuseFactor = max(dot(Normal, normalize(LightDirection[i])), 0.0);

	if (DiffuseFactor > 0.0) {

		DiffuseColor = Fragment_Diffuse * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

		/* Specular Component */
		float SpecularAngle = max(dot(Normal, normalize(HalfwayVector[i])), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, Fragment_SpecularConstant);                
		if(SpecularFactor > 0.0)
			SpecularColor = Fragment_Specular * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
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

		DiffuseColor = Fragment_Diffuse * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

		/* Specular Component */
		float SpecularAngle = max(dot(Normal, normalize(HalfwayVector[i])), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, Fragment_SpecularConstant);                               
		if(SpecularFactor > 0.0)
			SpecularColor = Fragment_Specular * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
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
	float DiffuseFactor = max(dot(Fragment_Normal, LightToVertex), 0.0);

	if (DiffuseFactor > 0) {

		/* SpotLight Circular effect fading around the edges */
		float CosineDifference = SPOTLIGHT_OUTER_ANGLE - LightSources[i].CutOff ;

		float SpotEffect = clamp((dot(normalize(LightDirection[i]), -LightToVertex) - LightSources[i].CutOff) / CosineDifference, 0.0, 1.0);

		DiffuseColor = Fragment_Diffuse * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor * SpotEffect;

		/* Specular Component */			                
		float SpecularAngle = max(dot(Fragment_Normal, normalize(HalfwayVector[i])), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, Fragment_SpecularConstant);                               
		if(SpecularFactor > 0.0)
			SpecularColor = Fragment_Specular *  LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor * SpotEffect;
	}

	/* Final Calculation */
	return AmbientColor + (DiffuseColor + SpecularColor) * LightIntensity;
}

void main() {

	/* Fragment Normal */
	Normal = normalize(Fragment_Normal);

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