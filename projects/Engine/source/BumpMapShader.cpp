#include "BumpMapShader.h"

BumpMapShader::BumpMapShader(string name) :
	ShaderProgram(name,BUMPMAP_VERTEX_SHADER_FILE,BUMPMAP_FRAGMENT_SHADER_FILE) {
}

BumpMapShader::~BumpMapShader() {

	ShaderProgram::~ShaderProgram();
}

void BumpMapShader::bindAttributes() {

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

void BumpMapShader::bindUniforms() {

	/* Binds Projection, View Matrices and Light Sources */
	ShaderProgram::bindUniforms();

	/*_uniformLocationMap["pointLight.Position"] = getUniformLocation("pointLight.Position");
	_uniformLocationMap["pointLight.Color"] = getUniformLocation("pointLight.Color");

	_uniformLocationMap["pointLight.AmbientIntensity"] = getUniformLocation("pointLight.AmbientIntensity");
	_uniformLocationMap["pointLight.DiffuseIntensity"] = getUniformLocation("pointLight.DiffuseIntensity");

	_uniformLocationMap["pointLight.Attenuation_Constant"] = getUniformLocation("pointLight.Attenuation_Constant");
	_uniformLocationMap["pointLight.Attenuation_Linear"] = getUniformLocation("pointLight.Attenuation_Linear");
	_uniformLocationMap["pointLight.Attenuation_Exp"] = getUniformLocation("pointLight.Attenuation_Exp");
	
	_uniformLocationMap["diffuse_tex"] = glGetUniformLocation(_programID, "diffuse_tex");
	_uniformLocationMap["normal_tex"] = glGetUniformLocation(_programID, "normal_tex");
	*/
	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}
