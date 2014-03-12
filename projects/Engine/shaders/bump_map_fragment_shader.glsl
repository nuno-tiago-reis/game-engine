#version 330 core
#pragma optionNV(unroll all)

#define LIGHT_COUNT 5

#define SPOT_LIGHT 1
#define POSITIONAL_LIGHT 2
#define DIRECTIONAL_LIGHT 3

/* Input Attributes */
in vec4 out_Position;

in vec3 out_Normal;
in vec3 out_Tangent;
in vec3 out_Bitangent;

in vec2 out_TextureUV;

in vec4 out_Ambient;
in vec4 out_Diffuse;
in vec4 out_Specular;
in float out_SpecularConstant;

in vec3 out_LightDirection[LIGHT_COUNT];
in vec3 out_HalfwayVector[LIGHT_COUNT];

in mat3 NormalMatrix;
in mat3 LightMatrix;

/* Uniforms */
uniform mat4 ModelMatrix;

uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;

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
	
	/* Light LightDistance / Direction */
	vec3 LightDirection = normalize(out_LightDirection[i]);
	float LightDistance = length(vec3((ViewMatrix * LightSources[i].Position) - out_Position));

	/* Light Intensity */
	float LightIntensity = 1.0 / (LightSources[i].ConstantAttenuation + LightSources[i].LinearAttenuation * LightDistance + LightSources[i].ExponentialAttenuation * LightDistance * LightDistance);

	/* Texture Component */
	vec4 BaseTexture = texture2D(DiffuseTexture, out_TextureUV);
	vec3 BumpNormal = normalize(texture2D(NormalTexture, out_TextureUV).xyz * 2.0 - 1.0);

	/* Ambient Component */
	vec4 AmbientColor = out_Diffuse * LightSources[i].Color * LightSources[i].AmbientIntensity;
	vec4 DiffuseColor = vec4(0, 0, 0, 1);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 1);

	/* Diffuse Component */
	float DiffuseFactor = max(dot(BumpNormal, LightDirection), 0.0);

	if (DiffuseFactor > 0.0) {

		DiffuseColor = out_Diffuse * BaseTexture * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

		/* Specular Component */
		vec3 HalfwayVector = normalize(out_HalfwayVector[i]);
		       
		float SpecularAngle = max(dot(BumpNormal, HalfwayVector), 0.0);

		float SpecularFactor = pow(SpecularAngle, out_SpecularConstant);                
		if(SpecularFactor > 0.0)
			SpecularColor = out_Specular * BaseTexture * LightSources[i].SpecularIntensity * SpecularFactor;

		//return vec4(SpecularFactor,0,0,1);
	}

	/* Final Calculation */
	return AmbientColor + (DiffuseColor + SpecularColor * 500) * LightIntensity;

	//return DiffuseColor * LightIntensity;
	//return vec4(0,0,0,1);

	//return vec4(DiffuseFactor,0,0,1);

	//return (vec4(out_Tangent,1) + vec4(1,1,1,1)) * 0.5;
	//return vec4(out_Tangent,1);
}

vec4 directionalLight(int i) {

	/* Vertex Normal */
	vec3 Normal = normalize(out_Normal);

	/* Light LightDistance / Direction */
	vec3 LightDirection = normalize(LightMatrix * vec3(LightSources[i].Direction));

	/* Texture Component */
	vec4 BaseTexture = texture2D(DiffuseTexture, out_TextureUV);
	vec3 BumpNormal = normalize(texture2D(NormalTexture, out_TextureUV).rgb * 2.0 - 1.0);

	/* Ambient Component */
	vec4 AmbientColor = out_Ambient * LightSources[i].Color * LightSources[i].AmbientIntensity;
	vec4 DiffuseColor = vec4(0, 0, 0, 1);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 1);

	/* Diffuse Component */
	float DiffuseFactor = max(dot(BumpNormal, -LightDirection), 0.0);

	if (DiffuseFactor > 0) {

		DiffuseColor = (out_Diffuse * BaseTexture) * LightSources[i].Color * LightSources[i].DiffuseIntensity * DiffuseFactor;

		/* Specular Component */
		vec3 HalfwayVector = normalize(LightDirection);

		float SpecularAngle = max(dot(BumpNormal, HalfwayVector), 0.0);
			                          
		float SpecularFactor = pow(SpecularAngle, out_SpecularConstant);                               
		if(SpecularFactor > 0.0)
			SpecularColor = out_Specular * LightSources[i].Color * LightSources[i].SpecularIntensity * SpecularFactor;
	}

	/* Final Calculation */
	return AmbientColor + DiffuseColor + SpecularColor;
}

vec4 spotLight(int i) {

	return vec4(0,0,0,1);
}

void main() {

	out_Color = vec4(0);
	
	for(int i=0; i<LIGHT_COUNT; i++) {

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