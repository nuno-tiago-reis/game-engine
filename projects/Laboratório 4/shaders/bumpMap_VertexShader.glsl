#version 330 core
#pragma optionNV(unroll all)

#define lightCount 10

in vec4 Position;
in vec4 Color;
in vec4 Normal;
in vec2 TexUV;
in vec4 Ambient;
in vec4 Tangent;
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

	LightSource lightSource[10];
};

mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
//mat4 NormalMatrix = transpose(inverse(ModelMatrix));

out vec4 vertPos;
out vec4 eyeModel;

out vec3 normalInterp;

out vec4 ambient;
out vec4 color;
out vec4 spec;
out vec2 texuv;
out vec4 lightPosition[lightCount];

void main() {

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;

    vertPos = ModelMatrix * Position;

    normalInterp = vec3(ModelMatrix * Normal);
	eyeModel = inverse(ViewMatrix) * vec4(0.0, 0.0, 0.0, 1.0);

	ambient = Ambient;
	color = Color;
	spec = Specular;
	texuv = TexUV;
	for(int i=0; i<lightCount; i++) {

		if(lightSource[i].lightType == 0)
			continue;

		lightPosition[i] = ViewMatrix * lightSource[i].Position;
	}
}