#include "MotionBlurShader.h"

MotionBlurShader::MotionBlurShader(string name) :
	ShaderProgram(name,MOTION_BLUR_VERTEX_SHADER_FILE, MOTION_BLUR_FRAGMENT_SHADER_FILE) {
}

MotionBlurShader::~MotionBlurShader() {

	ShaderProgram::~ShaderProgram();
}

void MotionBlurShader::bindAttributes() {

	/* Binds Motion Blur Vertex Attributes (Position) */
	glBindAttribLocation(_programID, POSITION, POSITION_ATTRIBUTE);

	Utility::checkOpenGLError("ERROR: Shader program \"" + _name + "\" attribute binding error.");
}

void MotionBlurShader::bindUniforms() {

	/* Binds Projection, View Matrices and Light Sources */
	//ShaderProgram::bindUniforms();

	_uniformLocationMap[TEXTURE_0_UNIFORM] = glGetUniformLocation(_programID, TEXTURE_0_UNIFORM);
	_uniformLocationMap[TEXTURE_1_UNIFORM] = glGetUniformLocation(_programID, TEXTURE_1_UNIFORM);

	/* Initialize Uniforms */
	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}