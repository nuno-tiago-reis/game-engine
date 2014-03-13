#version 330 core
#pragma optionNV(unroll all)

#define LIGHT_COUNT 5

#define SPOT_LIGHT 1
#define POSITIONAL_LIGHT 2
#define DIRECTIONAL_LIGHT 3

#define SPOTLIGHT_OUTER_ANGLE 0.97

/* Input Attributes (Same as the other Vertex Shaders) */
in vec4 Vertex_Position;

in vec4 Vertex_Normal;
in vec4 Vertex_Tangent;

in vec2 Vertex_TextureUV;

in vec4 Vertex_Ambient;
in vec4 Vertex_Diffuse;
in vec4 Vertex_Specular;
in float Vertex_SpecularConstant;

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

/* Output Attributes (Same as the Bump-Map Fragment Shader) */
out vec4 Fragment_Position;

out vec3 Fragment_Normal;
out vec3 Fragment_Tangent;
out vec3 Fragment_Bitangent;

out vec2 Fragment_TextureUV;

out vec4 Fragment_Ambient;
out vec4 Fragment_Diffuse;
out vec4 Fragment_Specular;
out float Fragment_SpecularConstant;

out vec3 LightDirection[LIGHT_COUNT];
out vec3 HalfwayVector[LIGHT_COUNT];

out mat3 NormalMatrix;
out mat3 LightMatrix;

/* Converts a Vector to Tangent Space */
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

	/* Vertex Position to Clip Space */
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Vertex_Position;

	/* Vertex Position, Normal, Tangent and Bitangent to View Space */
    Fragment_Position = ViewMatrix * ModelMatrix * Vertex_Position;

    Fragment_Normal = normalize(NormalMatrix * vec3(Vertex_Normal));
	Fragment_Tangent = normalize(NormalMatrix * vec3(Vertex_Tangent));
	Fragment_Bitangent = cross(Fragment_Normal,Fragment_Tangent) * Vertex_Tangent.w;

	/* Vertex Texture Coordinate */
	Fragment_TextureUV = Vertex_TextureUV;

	/* Vertex Material */
	Fragment_Ambient = Vertex_Ambient;
	Fragment_Diffuse = Vertex_Diffuse;
	Fragment_Specular = Vertex_Specular;
	Fragment_SpecularConstant = Vertex_SpecularConstant;

	/* Light computing */
	for(int i=0; i<LIGHT_COUNT; i++) {

		switch(LightSources[i].LightType) { 

			case POSITIONAL_LIGHT:	LightDirection[i] = convertToTangentSpace(
										vec3((ViewMatrix * LightSources[i].Position) - Fragment_Position), 
										Fragment_Tangent, Fragment_Bitangent, Fragment_Normal);

									HalfwayVector[i] = convertToTangentSpace(
										vec3(-Fragment_Position) + vec3((ViewMatrix * LightSources[i].Position) - Fragment_Position), 
										Fragment_Tangent, Fragment_Bitangent, Fragment_Normal);
									break;

			case DIRECTIONAL_LIGHT:	LightDirection[i] = convertToTangentSpace(
										LightMatrix * vec3(LightSources[i].Direction), 
										Fragment_Tangent, Fragment_Bitangent, Fragment_Normal);

									HalfwayVector[i] = LightDirection[i];
									break;
		
			case SPOT_LIGHT:		LightDirection[i] = convertToTangentSpace(
										LightMatrix * vec3(LightSources[i].Direction), 
										Fragment_Tangent, Fragment_Bitangent, Fragment_Normal);
									
									HalfwayVector[i] = convertToTangentSpace(
										vec3(-Fragment_Position) + vec3((ViewMatrix * LightSources[i].Position) - Fragment_Position), 
										Fragment_Tangent, Fragment_Bitangent, Fragment_Normal);
									break;
		}
	}
}