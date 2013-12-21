#version 330 core

in vec4 Position;
in vec4 Color;
in vec4 Normal;
in vec2 TexUV;
in vec4 Ambient;
in vec4 Tangent;
in vec4 Specular;
in float SpecularConstant;

uniform mat4 ModelMatrix;
layout(std140) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
mat4 NormalMatrix = transpose(inverse(ModelMatrix));

out vec3 normalInterp;
out vec3 vertPos;
out vec4 color;
out vec4 ambient;
out vec4 spec;
out vec2 texUV;


void main(){
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * Position;
	vec4 vertPos4 = ModelMatrix * Position;
    vertPos = vec3(vertPos4);// / vertPos4.w;
    normalInterp = vec3(NormalMatrix * Normal);
	color = Color;
	ambient = Ambient;
	spec = Specular;
	texUV=TexUV;
}