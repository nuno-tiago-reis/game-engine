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

uniform sampler3D Noise;
uniform float NoiseScale;

/* Output Attributes (Same as the Real-Wood Fragment Shader) */
out vec4 Fragment_Position;

out vec3 Fragment_Normal;

out vec4 Fragment_WoodColor;
out vec4 Fragment_ModelPosition;

out vec4 Fragment_Ambient;
out vec4 Fragment_Diffuse;
out vec4 Fragment_Specular;
out float Fragment_SpecularConstant;

out vec3 LightDirection[LIGHT_COUNT];
out vec3 HalfwayVector[LIGHT_COUNT];

void main() {

	/* Normal transformation matrices */
	mat3 NormalMatrix = inverse(transpose(mat3(ViewMatrix * ModelMatrix)));
	mat3 LightMatrix = inverse(transpose(mat3(ViewMatrix)));

	/* Vertex Position to Clip Space */
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Vertex_Position;

	/* Vertex Position and Normal to View Space */
    Fragment_Position = ViewMatrix * ModelMatrix * Vertex_Position;
    Fragment_Normal = NormalMatrix * vec3(Vertex_Normal);

	/******************************************* WOOD MAPPING ********************************************************/
	/* Wood Color Calculations * /
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
	
	vec3 RotatedPosition =  (RotationMatrix * ModelMatrix * Vertex_Position * NoiseScale).xyz;
	float RotatedNoise = 2.0 * texture(Noise, RotatedPosition).r - 1.0;
	float r = fract(RepetitionRate * ((ModelMatrix * Vertex_Position).y * NoiseScale) + RepetitionScale * RotatedNoise);
	float invMax = pow(RepetitionSharpness, RepetitionSharpness / (RepetitionSharpness - 1.0))/(RepetitionSharpness - 1.0);
	float ring = invMax * (r - pow(r, RepetitionSharpness));
	float lerp = ring + RotatedNoise;
	
	Fragment_WoodColor = mix(DarkShade, LightShade, lerp);
	/ *****************************************************************************************************************/
	Fragment_ModelPosition = ModelMatrix * Vertex_Position;

	/* Vertex Material */
	Fragment_Ambient = Vertex_Ambient;
	Fragment_Diffuse = Vertex_Diffuse;
	Fragment_Specular = Vertex_Specular;
	Fragment_SpecularConstant = Vertex_SpecularConstant;

	/* Light computing */
	for(int i=0; i<LIGHT_COUNT; i++) {

		switch(LightSources[i].LightType) { 

			case POSITIONAL_LIGHT:	LightDirection[i] = vec3((ViewMatrix * LightSources[i].Position) - Fragment_Position);
									HalfwayVector[i] = vec3(-Fragment_Position) + LightDirection[i];
									break;
			
			case DIRECTIONAL_LIGHT:	LightDirection[i] = LightMatrix * vec3(LightSources[i].Direction);
									HalfwayVector[i] = LightDirection[i];
									break;

			case SPOT_LIGHT:		LightDirection[i] = LightMatrix * vec3(LightSources[i].Direction);
									HalfwayVector[i] = vec3(-Fragment_Position) + LightDirection[i];
									break;
		}
	}
}