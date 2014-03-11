#version 330 core
#pragma optionNV(unroll all)

#define LIGHT_COUNT 10

#define SPOT_LIGHT 1
#define POSITIONAL_LIGHT 2
#define DIRECTIONAL_LIGHT 3

/* Input Attributes */
in vec4 out_Position;

in vec3 out_Normal;

in vec2 out_TextureUV;

in vec4 out_Ambient;
in vec4 out_Diffuse;
in vec4 out_Specular;
in float out_SpecularConstant;

in mat3 NormalMatrix;
in mat3 LightMatrix;

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

/* Output Attributes */
out vec4 out_Color;

vec4 positionalLight(int i) {

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

		DiffuseColor = out_Diffuse * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

		/* Specular Component */
		vec3 HalfwayVector = normalize(vec3(-out_Position) + LightDirection);
			                
		float SpecularAngle = max(dot(Normal, HalfwayVector), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, out_SpecularConstant);                
		if(SpecularFactor > 0.0)
			SpecularColor = out_Specular * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
	}

	/* Final Calculation */
	return AmbientColor + (DiffuseColor + SpecularColor) * LightIntensity;
}

vec4 directionalLight(int i) {

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

		DiffuseColor = out_Diffuse * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

		/* Specular Component */
		vec3 HalfwayVector = normalize(LightDirection);

		float SpecularAngle = max(dot(Normal, HalfwayVector), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, out_SpecularConstant);                               
		if(SpecularFactor > 0.0)
			SpecularColor = out_Specular * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
	}

	/* Final Calculation */
	return AmbientColor + DiffuseColor + SpecularColor;
}

vec4 spotLight(int i) {

	/* Vertex Normal */
	vec3 Normal = out_Normal;

	/* Light LightDistance / Direction */
	vec3 LightToVertex = vec3(ViewMatrix * LightSources[i].Position - out_Position);
	float LightDistance = length(LightToVertex);  
	LightToVertex = normalize(LightToVertex);

	//vec4 LightDirection = normalize(ViewMatrix * LightSources[i].Direction);
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

			DiffuseColor = out_Diffuse * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

			/* Specular Component */
			vec3 HalfwayVector = normalize(vec3(-out_Position)) + normalize(vec3(ViewMatrix * LightSources[i].Position - out_Position));
			HalfwayVector = normalize(HalfwayVector);
			                
			float SpecularAngle = max(dot(Normal, HalfwayVector), 0.0);
			                          
			float SpecularFactor = pow(SpecularAngle, out_SpecularConstant);                               
			if(SpecularFactor > 0.0)
				SpecularColor = out_Specular * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
		}
	}

	return AmbientColor + (DiffuseColor + SpecularColor) * LightIntensity;
}

void main() {

	out_Color = vec4(0);
	
	for(int i=0; i<LIGHT_COUNT; i++) {

		if(LightSources[i].LightType == 0)
			continue;

		switch(LightSources[i].LightType) { 
		
			case SPOT_LIGHT:		out_Color += spotLight(i);
									break;
			
			case DIRECTIONAL_LIGHT:	out_Color += directionalLight(i);
									break;

			case POSITIONAL_LIGHT:	out_Color += positionalLight(i);
									break;
		}
	}
}