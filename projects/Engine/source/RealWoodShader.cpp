#include "RealWoodShader.h"

RealWoodShader::RealWoodShader(string name) : 
	ShaderProgram(name,WOOD_TEXTURE_VERTEX_SHADER_FILE,WOOD_TEXTURE_FRAGMENT_SHADER_FILE) {
}

RealWoodShader::~RealWoodShader(){

	ShaderProgram::~ShaderProgram();
}

void RealWoodShader::bindAttributes() {

	glBindAttribLocation(_programID, POSITION, POSITION_ATTRIBUTE);

	glBindAttribLocation(_programID, TEXTURE_UV, TEXTURE_UV_ATTRIBUTE);

	glBindAttribLocation(_programID, NORMAL, NORMAL_ATTRIBUTE);
	glBindAttribLocation(_programID, TANGENT, TANGENT_ATTRIBUTE);

	glBindAttribLocation(_programID, AMBIENT, AMBIENT_ATTRIBUTE);
	glBindAttribLocation(_programID, DIFFUSE, DIFFUSE_ATTRIBUTE);
	glBindAttribLocation(_programID, SPECULAR, SPECULAR_ATTRIBUTE);
	glBindAttribLocation(_programID, SPECULAR_CONSTANTS, SPECULAR_CONSTANT_ATTRIBUTE);

	Utility::checkOpenGLError("ERROR: Shader program \"" + _name + "\" attribute binding error.");
}

void RealWoodShader::bindUniforms() {

	/* Binds Projection, View Matrices and Light Sources */
	ShaderProgram::bindUniforms();

	_uniformLocationMap["Noise"] = glGetUniformLocation(_programID, "Noise");
	_uniformLocationMap["NoiseScale"] = glGetUniformLocation(_programID, "NoiseScale");

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}