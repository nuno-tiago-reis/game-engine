#version 330 core
#pragma optionNV(unroll all)

#define LIGHT_COUNT 10

#define SPOT_LIGHT 1
#define POSITIONAL_LIGHT 2
#define DIRECTIONAL_LIGHT 3

in vec4 Position;

in vec4 Normal;
in vec4 Tangent;

in vec2 TextureUV;

in vec4 Ambient;
in vec4 Diffuse;
in vec4 Specular;
in float SpecularConstant;

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

out vec4 out_Position;

out vec2 out_TextureUV;

out vec3 out_Normal;

out vec4 out_Ambient;
out vec4 out_Diffuse;
out vec4 out_Specular;

out float out_SpecularConstant;

out mat3 NormalMatrix;
out mat3 LightMatrix;

void main() {

	NormalMatrix = inverse(transpose(mat3(ViewMatrix * ModelMatrix)));
	LightMatrix = inverse(transpose(mat3(ViewMatrix)));

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;

    out_Position = ViewMatrix * ModelMatrix * Position;
	out_Normal = NormalMatrix * vec3(Normal);
	
	/****/
	vec3 VertexToEye = normalize(vec3(out_Position));
	vec3 Reflect = reflect(VertexToEye, out_Normal);

	float m = 2.0 * sqrt(Reflect.x * Reflect.x + Reflect.y * Reflect.y + (Reflect.z + 1.0) * (Reflect.z + 1.0));

	out_TextureUV = vec2(Reflect.x / m + 0.5, Reflect.y / m + 0.5);
	/**********/

	out_Ambient = Ambient;
	out_Diffuse = Diffuse;
	out_Specular = Specular;
	out_SpecularConstant = SpecularConstant;
}