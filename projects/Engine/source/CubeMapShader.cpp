#include "CubeMapShader.h"

CubeMapShader::CubeMapShader(string name) :
	ShaderProgram(name,CUBE_MAP_VERTEX_SHADER_FILE,CUBE_MAP_FRAGMENT_SHADER_FILE) {
}

CubeMapShader::~CubeMapShader() {

	ShaderProgram::~ShaderProgram();
}

void CubeMapShader::bindAttributes() {

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

void CubeMapShader::bindUniforms() {

	/* Binds Projection, View Matrices and Light Sources */
	ShaderProgram::bindUniforms();

	_uniformLocationMap[CUBE_MAP_UNIFORM] = glGetUniformLocation(_programID, CUBE_MAP_UNIFORM);

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}