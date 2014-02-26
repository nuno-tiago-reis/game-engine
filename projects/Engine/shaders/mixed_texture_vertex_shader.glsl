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

	int lightType;
};

layout(std140) uniform LightSources {

	LightSource lightSource[LIGHT_COUNT];
};

mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;

out vec4 out_Position;
out vec4 out_CameraPosition;

out vec2 out_TextureUV;

out vec3 out_Normal;

out vec4 out_Ambient;
out vec4 out_Diffuse;
out vec4 out_Specular;

out vec4 lightPosition[LIGHT_COUNT];

void main() {

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;

    out_Position = ModelMatrix * Position;
	out_CameraPosition = inverse(ViewMatrix) * vec4(0.0, 0.0, 0.0, 1.0);
	
	out_TextureUV = TextureUV;

    out_Normal = vec3(ModelMatrix * Normal);

	out_Ambient = Ambient;
	out_Diffuse = Diffuse;
	out_Specular = Specular;
	
	for(int i=0; i<LIGHT_COUNT; i++) {

		if(lightSource[i].lightType == 0)
			continue;

		lightPosition[i] = ViewMatrix * lightSource[i].Position;
	}
}