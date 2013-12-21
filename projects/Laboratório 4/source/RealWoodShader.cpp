#include "RealWoodShader.h"

RealWoodShader::RealWoodShader(const char* name) : 
	ShaderProgram(name,WOOD_TEXTURE_VERTEX_SHADER_FILE,WOOD_TEXTURE_FRAGMENT_SHADER_FILE) {
}

RealWoodShader::~RealWoodShader(){

	ShaderProgram::~ShaderProgram();
}

void RealWoodShader::bindAttributes() {

	glBindAttribLocation(_programID, VERTICES, "Position");
	glBindAttribLocation(_programID, COLORS, "Color");

	glBindAttribLocation(_programID, TEXUVS, "TexUV");

	glBindAttribLocation(_programID, NORMALS, "Normal");
	glBindAttribLocation(_programID, TANGENTS, "Tangent");

	glBindAttribLocation(_programID, AMBIENTS, "Ambient");
	glBindAttribLocation(_programID, SPECULARS, "Specular");
	glBindAttribLocation(_programID, SPECULARS_CONSTANTS, "SpecularConstant");

	Utility::checkOpenGLError("ERROR: Shader program \"" + _name + "\" attribute binding error.");
}

void RealWoodShader::bindUniforms() {
	ntname="Noise";
	ntloc = glGetUniformLocation(_programID, ntname);
	nsname="noiseScale";
	nsloc = glGetUniformLocation(_programID, nsname);
	ShaderProgram::bindUniforms();

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}