#version 330 core
#pragma optionNV(unroll all)

#define LIGHT_COUNT 10

#define SPOT_LIGHT 1
#define POSITIONAL_LIGHT 2
#define DIRECTIONAL_LIGHT 3

in vec4 out_Position;

in vec4 out_ModelSpacePosition;

in vec2 out_TextureUV;

in vec3 out_Normal;

in vec4 out_Ambient;
in vec4 out_Diffuse;
in vec4 out_Specular;
in float out_SpecularConstant;

in mat3 NormalMatrix;
in mat3 LightMatrix;

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

uniform mat4 ModelMatrix;

uniform sampler3D Noise;
uniform float NoiseScale;

out vec4 out_Color;

vec4 positionalLight(int i, vec4 NoiseColor) {

	/* Vertex Normal */
	vec3 Normal = out_Normal;
	
	/* Light LightDistance / Direction */
	vec3 LightDirection = vec3((ViewMatrix * LightSources[i].Position) - out_Position);
	float LightDistance = length(LightDirection);  
	LightDirection = normalize(LightDirection);

	/* Light Intensity */
	float LightIntensity = 1.0 / (LightSources[i].ConstantAttenuation + LightSources[i].LinearAttenuation * LightDistance + LightSources[i].ExponentialAttenuation * LightDistance * LightDistance);

	/* Ambient Component */
	vec4 AmbientColor = out_Ambient * LightSources[i].Color * LightSources[i].AmbientIntensity;
	vec4 DiffuseColor = vec4(0, 0, 0, 1);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 1);

	/* Diffuse Component */
	float DiffuseFactor = max(dot(Normal, LightDirection), 0.0);

	if (DiffuseFactor > 0.0) {

		DiffuseColor = (out_Diffuse * NoiseColor) * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

		/* Specular Component */
		vec3 HalfwayVector = normalize(vec3(-out_Position) + LightDirection);
			                
		float SpecularAngle = max(dot(HalfwayVector, Normal), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, out_SpecularConstant);                
		if(SpecularFactor > 0.0)
			SpecularColor = out_Specular * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
	}

	/* Final Calculation */
	return AmbientColor + (DiffuseColor + SpecularColor) * LightIntensity;
}

vec4 directionalLight(int i, vec4 NoiseColor) {

	/* Vertex Normal */
	vec3 Normal = out_Normal;

	/* Light LightDistance / Direction */
	vec3 LightDirection = normalize(LightMatrix * vec3(LightSources[i].Direction));

	/* Ambient Component */
	vec4 AmbientColor = out_Ambient * LightSources[i].Color * LightSources[i].AmbientIntensity;
	vec4 DiffuseColor = vec4(0, 0, 0, 1);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 1);

	/* Diffuse Component */
	float DiffuseFactor = max(dot(Normal, -LightDirection), 0.0);

	if (DiffuseFactor > 0) {

		DiffuseColor = (out_Diffuse * NoiseColor) * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

		/* Specular Component */
		vec3 HalfwayVector = normalize(LightDirection);

		float SpecularAngle = max(dot(HalfwayVector, Normal), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, out_SpecularConstant);                               
		if(SpecularFactor > 0.0)
			SpecularColor = out_Specular * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
	}

	/* Final Calculation */
	return AmbientColor + DiffuseColor + SpecularColor;

	return vec4(LightDirection,1);
}

vec4 spotLight(int i, vec4 NoiseColor) {

	/* Vertex Normal */
	vec3 Normal = out_Normal;

	/* Light LightDistance / Direction */
	vec3 LightToVertex = vec3(ViewMatrix * LightSources[i].Position - out_Position);
	float LightDistance = length(LightToVertex);  
	LightToVertex = normalize(LightToVertex);

	vec3 LightDirection = normalize(LightMatrix * vec3(LightSources[i].Direction));

	/* Light Intensity */
	float LightIntensity = 1.0 / (LightSources[i].ConstantAttenuation + LightSources[i].LinearAttenuation * LightDistance + LightSources[i].ExponentialAttenuation * LightDistance * LightDistance);

	/* Ambient Component */
	vec4 AmbientColor = out_Ambient * LightSources[i].Color * LightSources[i].AmbientIntensity;
	vec4 DiffuseColor = vec4(0, 0, 0, 0);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 0);
	
	/* Diffuse Component */
	float DiffuseFactor = max(dot(Normal, LightToVertex), 0.0);

	if (DiffuseFactor > 0) {

		float spotEffect = dot(LightDirection, -LightToVertex);

		if (spotEffect > cos(radians(LightSources[i].CutOff))) {

			spotEffect = pow(spotEffect,1.5);

			LightIntensity = spotEffect / (LightSources[i].ConstantAttenuation + LightSources[i].LinearAttenuation * LightDistance + LightSources[i].ExponentialAttenuation * LightDistance * LightDistance);

			DiffuseColor = (out_Diffuse * NoiseColor) * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

			/* Specular Component */
			vec3 HalfwayVector = normalize(vec3(-out_Position)) + normalize(vec3(ViewMatrix * LightSources[i].Position - out_Position));
			HalfwayVector = normalize(HalfwayVector);
			                
			float SpecularAngle = max(dot(HalfwayVector, Normal), 0.0);
			                          
			float SpecularFactor = pow(SpecularAngle, out_SpecularConstant);                               
			if(SpecularFactor > 0.0)
				SpecularColor = out_Specular * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
		}
	}

	return AmbientColor + (DiffuseColor + SpecularColor) * LightIntensity;
}

void main() {

	vec4 DarkShade=vec4(0.4, 0.2, 0.07, 1.0);
	vec4 LightShade=vec4(0.6, 0.3, 0.1, 1.0);
	float RepetitionRate=20;
	float RepetitionScale=3.14;
	float RepetitionSharpness=4352.0;

	mat4 RotationMatrix;
	RotationMatrix[0]=vec4(0.75,-0.21651,0.625,0);
	RotationMatrix[1]=vec4(0.43301,0.875,-0.21651,0);
	RotationMatrix[2]=vec4(-0.5,0.43301,0.75,0);
	RotationMatrix[3]=vec4(0,0,0,1);
	
	vec3 RotatedPosition =  (RotationMatrix * out_ModelSpacePosition * NoiseScale).xyz;
	float RotatedNoise = 2.0 * texture(Noise, RotatedPosition).r - 1.0;
	float r = fract(RepetitionRate * (out_ModelSpacePosition.z * NoiseScale) + RepetitionScale * RotatedNoise);
	float invMax = pow(RepetitionSharpness, RepetitionSharpness / (RepetitionSharpness - 1.0))/(RepetitionSharpness - 1.0);
	float ring = invMax * (r - pow(r, RepetitionSharpness));
	float lerp = ring + RotatedNoise;
	
	vec4 NoiseColor = mix(DarkShade, LightShade, lerp);

	out_Color = vec4(0);
	
	for(int i=0; i<LIGHT_COUNT; i++) {

		if(LightSources[i].LightType == 0)
			continue;

		switch(LightSources[i].LightType) { 
		
			case SPOT_LIGHT:		out_Color += spotLight(i,NoiseColor);
									break;
			
			case DIRECTIONAL_LIGHT:	out_Color += directionalLight(i,NoiseColor);
									break;

			case POSITIONAL_LIGHT:	out_Color += positionalLight(i,NoiseColor);
									break;
		}
	}
}