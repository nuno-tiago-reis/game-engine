#include "BlinnPhongShader.h"

BlinnPhongShader::BlinnPhongShader(const char* name) : 
	ShaderProgram(name,BLINN_PHONG_VERTEX_SHADER_FILE,BLINN_PHONG_FRAGMENT_SHADER_FILE) {
}

BlinnPhongShader::~BlinnPhongShader() {

	ShaderProgram::~ShaderProgram();
}

void BlinnPhongShader::bindAttributes() {

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

void BlinnPhongShader::bindUniforms() {

	//Binds Projection and View Matrices
	ShaderProgram::bindUniforms();

	/*_uniformLocationMap["pointLight.Position"] = getUniformLocation("pointLight.Position");
	_uniformLocationMap["pointLight.Color"] = getUniformLocation("pointLight.Color");

	_uniformLocationMap["pointLight.AmbientIntensity"] = getUniformLocation("pointLight.AmbientIntensity");
	_uniformLocationMap["pointLight.DiffuseIntensity"] = getUniformLocation("pointLight.DiffuseIntensity");

	_uniformLocationMap["pointLight.Attenuation_Constant"] = getUniformLocation("pointLight.Attenuation_Constant");
	_uniformLocationMap["pointLight.Attenuation_Linear"] = getUniformLocation("pointLight.Attenuation_Linear");
	_uniformLocationMap["pointLight.Attenuation_Exp"] = getUniformLocation("pointLight.Attenuation_Exp");*/

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}
