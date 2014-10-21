#include "MotionBlurShader.h"

MotionBlurShader::MotionBlurShader(string name) :
	ShaderProgram(name, MOTION_BLUR_VERTEX_SHADER_FILE, MOTION_BLUR_FRAGMENT_SHADER_FILE) {
}

MotionBlurShader::~MotionBlurShader() {
}

void MotionBlurShader::bindAttributes() {

	/* Binds Motion Blur Vertex Attributes (Position) */
	glBindAttribLocation(_programID, POSITION, POSITION_ATTRIBUTE);

	Utility::checkOpenGLError("ERROR: Shader program \"" + this->name + "\" attribute binding error.");
}

void MotionBlurShader::bindUniforms() {

	/* Binds Projection, View Matrices and Light Sources */
	this->uniformLocationMap[FRAME_0_TEXTURE_UNIFORM] = glGetUniformLocation(_programID, FRAME_0_TEXTURE_UNIFORM);
	this->uniformLocationMap[FRAME_1_TEXTURE_UNIFORM] = glGetUniformLocation(_programID, FRAME_1_TEXTURE_UNIFORM);

	/* Initialize Uniforms */
	Utility::checkOpenGLError("ERROR: Shader Program \"" + this->name + "\" uniform binding error.");
}