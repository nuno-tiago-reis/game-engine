#include "BumpMapShader.h"

BumpMapShader::BumpMapShader(string name) :
	ShaderProgram(name,BUMPMAP_VERTEX_SHADER_FILE,BUMPMAP_FRAGMENT_SHADER_FILE) {
}

BumpMapShader::~BumpMapShader() {

	ShaderProgram::~ShaderProgram();
}

void BumpMapShader::bindAttributes() {

	/* Binds Generic Vertex Attributes (Position, TextureUV, Normal, Tangent and Material) */
	glBindAttribLocation(_programID, POSITION, POSITION_ATTRIBUTE);

	glBindAttribLocation(_programID, TEXTURE_UV, TEXTURE_UV_ATTRIBUTE);

	glBindAttribLocation(_programID, NORMAL, NORMAL_ATTRIBUTE);
	glBindAttribLocation(_programID, TANGENT, TANGENT_ATTRIBUTE);

	glBindAttribLocation(_programID, AMBIENT, AMBIENT_ATTRIBUTE);
	glBindAttribLocation(_programID, DIFFUSE, DIFFUSE_ATTRIBUTE);
	glBindAttribLocation(_programID, SPECULAR, SPECULAR_ATTRIBUTE);
	glBindAttribLocation(_programID, SHININESS, SHININESS_ATTRIBUTE);

	Utility::checkOpenGLError("ERROR: Shader program \"" + _name + "\" attribute binding error.");
}

void BumpMapShader::bindUniforms() {

	/* Binds Projection, View Matrices and Light Sources */
	ShaderProgram::bindUniforms();
	
	_uniformLocationMap[DIFFUSE_TEXTURE_UNIFORM] = glGetUniformLocation(_programID, DIFFUSE_TEXTURE_UNIFORM);
	_uniformLocationMap[NORMAL_TEXTURE_UNIFORM] = glGetUniformLocation(_programID, NORMAL_TEXTURE_UNIFORM);

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");

	cout << "Diffuse Texture Uniform" << _uniformLocationMap[DIFFUSE_TEXTURE_UNIFORM] << endl;
	cout << "Normal Texture Uniform" << _uniformLocationMap[NORMAL_TEXTURE_UNIFORM] << endl;
}