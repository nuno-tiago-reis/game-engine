#version 330 core
#pragma optionNV(unroll all)

#define LIGHT_COUNT 10

#define SPOT_LIGHT 1
#define POSITIONAL_LIGHT 2
#define DIRECTIONAL_LIGHT 3

/* Input Attributes */
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

/* Output Attributes */
out vec4 out_Position;

out vec3 out_Normal;

out vec3 out_TextureCubeXYZ;

out vec4 out_Ambient;
out vec4 out_Diffuse;
out vec4 out_Specular;
out float out_SpecularConstant;

out mat3 NormalMatrix;
out mat3 LightMatrix;

void main() {

	/* Vertex Position to Clip Space */
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;

	/* Normal transformation matrices */
	NormalMatrix = inverse(transpose(mat3(ViewMatrix * ModelMatrix)));
	LightMatrix = inverse(transpose(mat3(ViewMatrix)));

	/* Vertex Position and Normal to View Space */
    out_Position = ViewMatrix * ModelMatrix * Position;
	
	out_Normal = normalize(NormalMatrix * vec3(Normal));
	
	/***** CUBE MAPPING *****/
	vec3 VertexToEye = normalize(vec3(out_Position));

	//out_TextureCubeXYZ = reflect(VertexToEye, inverse(transpose(mat3(ModelMatrix))) * vec3(Normal));

	out_TextureCubeXYZ = reflect(VertexToEye, out_Normal);

	out_TextureCubeXYZ = out_TextureCubeXYZ;
	/**********/

	/* Vertex Material */
	out_Ambient = Ambient;
	out_Diffuse = Diffuse;
	out_Specular = Specular;
	out_SpecularConstant = SpecularConstant;
}