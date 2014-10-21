#include "BloomShader.h"

BloomShader::BloomShader(string name) :
	ShaderProgram(name, BLOOM_VERTEX_SHADER_FILE, BLOOM_FRAGMENT_SHADER_FILE) {
}

BloomShader::~BloomShader() {
}

void BloomShader::bindAttributes() {

	/* Binds Motion Blur Vertex Attributes (Position) */
	glBindAttribLocation(_programID, POSITION, POSITION_ATTRIBUTE);

	Utility::checkOpenGLError("ERROR: Shader program \"" + this->name + "\" attribute binding error.");
}

void BloomShader::bindUniforms() {

	/* Binds Projection, View Matrices and Light Sources */
	this->uniformLocationMap[COLOR_TEXTURE_UNIFORM] = glGetUniformLocation(_programID, COLOR_TEXTURE_UNIFORM);
	this->uniformLocationMap[BRIGHTNESS_TEXTURE_UNIFORM] = glGetUniformLocation(_programID, BRIGHTNESS_TEXTURE_UNIFORM);

	/* Initialize Uniforms */
	Utility::checkOpenGLError("ERROR: Shader Program \"" + this->name + "\" uniform binding error.");
}