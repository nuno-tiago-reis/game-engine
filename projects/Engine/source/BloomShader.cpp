#include "BloomShader.h"

BloomShader::BloomShader(string name) :
	ShaderProgram(name, BLOOM_VERTEX_SHADER_FILE, BLOOM_FRAGMENT_SHADER_FILE) {
}

BloomShader::~BloomShader() {
}

void BloomShader::bindAttributes() {

	/* Binds Motion Blur Vertex Attributes (Position) */
	glBindAttribLocation(_programID, POSITION, POSITION_ATTRIBUTE);

	Utility::checkOpenGLError("ERROR: Shader program \"" + _name + "\" attribute binding error.");
}

void BloomShader::bindUniforms() {

	/* Binds Projection, View Matrices and Light Sources */
	_uniformLocationMap[TEXTURE_0_UNIFORM] = glGetUniformLocation(_programID, TEXTURE_0_UNIFORM);
	_uniformLocationMap[TEXTURE_1_UNIFORM] = glGetUniformLocation(_programID, TEXTURE_1_UNIFORM);

	/* Initialize Uniforms */
	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}