#include "MixedTextureShader.h"

MixedTextureShader::MixedTextureShader(string name) :
	ShaderProgram(name,MIXED_TEXTURE_VERTEX_SHADER_FILE,MIXED_TEXTURE_FRAGMENT_SHADER_FILE) {
}

MixedTextureShader::~MixedTextureShader() {
}

void MixedTextureShader::bindAttributes() {

	glBindAttribLocation(_programID, POSITION, POSITION_ATTRIBUTE);

	//glBindAttribLocation(_programID, TEXTURE_UV, TEXTURE_UV_ATTRIBUTE);

	glBindAttribLocation(_programID, NORMAL, NORMAL_ATTRIBUTE);
	//glBindAttribLocation(_programID, TANGENT, TANGENT_ATTRIBUTE);

	glBindAttribLocation(_programID, AMBIENT, AMBIENT_ATTRIBUTE);
	glBindAttribLocation(_programID, DIFFUSE, DIFFUSE_ATTRIBUTE);
	glBindAttribLocation(_programID, SPECULAR, SPECULAR_ATTRIBUTE);
	//glBindAttribLocation(_programID, SPECULAR_CONSTANTS, SPECULAR_CONSTANT_ATTRIBUTE);

	Utility::checkOpenGLError("ERROR: Shader program \"" + _name + "\" attribute binding error.");
}

void MixedTextureShader::bindUniforms() {

	/* Binds Projection, View Matrices and Light Sources */
	ShaderProgram::bindUniforms();

	_uniformLocationMap["Texture0"] = glGetUniformLocation(_programID, "Texture0");
	_uniformLocationMap["Texture1"] = glGetUniformLocation(_programID, "Texture1");

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}