#version 330 core
#pragma optionNV(unroll all)

#define LIGHT_COUNT 5

#define SPOT_LIGHT 1
#define POSITIONAL_LIGHT 2
#define DIRECTIONAL_LIGHT 3

/* Input Variables */
in vec4 Position;

in vec4 Normal;
in vec4 Tangent;

in vec2 TextureUV;

in vec4 Ambient;
in vec4 Diffuse;
in vec4 Specular;
in float SpecularConstant;

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

/* Output Variables */
out vec4 out_Position;

out vec3 out_Normal;
out vec3 out_Tangent;
out vec3 out_Bitangent;

out vec2 out_TextureUV;

out vec4 out_Ambient;
out vec4 out_Diffuse;
out vec4 out_Specular;
out float out_SpecularConstant;

out vec3 out_LightDirection[LIGHT_COUNT];
out vec3 out_HalfwayVector[LIGHT_COUNT];

out mat3 NormalMatrix;
out mat3 LightMatrix;

vec3 convertToTangentSpace(vec3 Vector, vec3 Tangent, vec3 Bitangent, vec3 Normal) {

	vec3 Result;

	Result.x = dot(Vector, Tangent);
	Result.y = dot(Vector, Bitangent);
	Result.z = dot(Vector, Normal);

	return normalize(Result);
}

void main() {

	/* Normal transformation matrices */
	NormalMatrix = inverse(transpose(mat3(ViewMatrix * ModelMatrix)));
	LightMatrix = inverse(transpose(mat3(ViewMatrix)));

	/* Vertex Position computing */
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;

    out_Position = ViewMatrix * ModelMatrix * Position;
	
	/* Normal Space computing */
    out_Normal = normalize(NormalMatrix * vec3(Normal));
	out_Tangent = normalize(NormalMatrix * vec3(Tangent));
	out_Bitangent = cross(out_Normal,out_Tangent);

	/* Texture  Coordinate */
	out_TextureUV = TextureUV;

	/* Temporary Variables */
	vec3 LightDirection;
	vec3 HalfwayVector;

	/* Light computing */
	for(int i=0; i<LIGHT_COUNT; i++) {

		switch(LightSources[i].LightType) { 
		
			case SPOT_LIGHT:		//vec3 LightDirection = normalize(LightMatrix * vec3(LightSources[i].Direction));
									LightDirection = normalize(LightMatrix * vec3(LightSources[i].Direction));

									out_LightDirection[i] = convertToTangentSpace(LightDirection, out_Tangent, out_Bitangent, out_Normal);
									
									//vec3 HalfwayVector = normalize(vec3(-out_Position)) + normalize(vec3(ViewMatrix * LightSources[i].Position - out_Position));
									HalfwayVector = normalize(vec3(-out_Position) + LightDirection);

									out_HalfwayVector[i] = convertToTangentSpace(HalfwayVector, out_Tangent, out_Bitangent, out_Normal);
									break;
			
			case DIRECTIONAL_LIGHT:	//vec3 LightDirection = normalize(LightMatrix * vec3(LightSources[i].Direction));
									LightDirection = normalize(LightMatrix * vec3(LightSources[i].Direction));

									out_LightDirection[i] = convertToTangentSpace(LightDirection, out_Tangent, out_Bitangent, out_Normal);
									
									//vec3 HalfwayVector = normalize(LightDirection);
									HalfwayVector = LightDirection;

									out_HalfwayVector[i] = convertToTangentSpace(HalfwayVector, out_Tangent, out_Bitangent, out_Normal);
									break;

			case POSITIONAL_LIGHT:	//vec3 LightDirection = vec3((ViewMatrix * LightSources[i].Position) - out_Position);
									LightDirection = normalize(vec3((ViewMatrix * LightSources[i].Position) - out_Position));

									out_LightDirection[i] = convertToTangentSpace(LightDirection, out_Tangent, out_Bitangent, out_Normal);
									
									//vec3 HalfwayVector = normalize(vec3(-out_Position) + LightDirection);
									HalfwayVector = normalize(vec3(-out_Position) + LightDirection);

									out_HalfwayVector[i] = convertToTangentSpace(HalfwayVector, out_Tangent, out_Bitangent, out_Normal);
									break;
		}
	}

	/* Material Computing */
	out_Ambient = Ambient;
	out_Diffuse = Diffuse;
	out_Specular = Specular;
	out_SpecularConstant = SpecularConstant;
}