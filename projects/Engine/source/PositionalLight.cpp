#include "PositionalLight.h"

PositionalLight::PositionalLight(string name) : Light(name) {

	_constantAttenuation = 0.0f;
	_linearAttenuation = 0.0f;
	_exponentialAttenuation = 0.0f;
}

PositionalLight::~PositionalLight() {
}

void PositionalLight::loadUniforms() {

	GLfloat lightSourceArray[LIGHT_SOURCE_STRUCT_SIZE];

	/* Position */
	for(int i=0; i<4; i++)
		lightSourceArray[i] = _position[i];
	/* Direction */
	for(int i=0; i<4; i++)
		lightSourceArray[i+4] = 0.0f;
	/* Color */
	for(int i=0; i<4; i++)
		lightSourceArray[i+8] = _color[i];

	/* Cut Off */
	lightSourceArray[12] = 0.0f;
	/* Intensity (Ambient, Diffuse, Specular) */
	lightSourceArray[13] = _ambientIntensity;
	lightSourceArray[14] = _diffuseIntensity;
	lightSourceArray[15] = _specularIntensity;
	/* Attenuation (Constant, Linear, Exponential) */
	lightSourceArray[16] = _constantAttenuation;
	lightSourceArray[17] = _linearAttenuation;
	lightSourceArray[18] = _exponentialAttenuation;

	GLint lightType = POSITIONAL_LIGHT;

	/* Uniform Locations */
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferIndex);

	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * _identifier, 
										sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE, lightSourceArray);
	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * _identifier + sizeof(GLint) * LIGHT_SOURCE_STRUCT_SIZE, 
										sizeof(GLint), &lightType);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Utility::checkOpenGLError("ERROR: PositionalLight \"" + _name + "\" Uniform loading failed.");
}

GLfloat PositionalLight::getConstantAttenuation() {

	return _constantAttenuation;
}

GLfloat PositionalLight::getLinearAttenuation() {

	return _linearAttenuation;
}

GLfloat PositionalLight::getExponentinalAttenuation() {

	return _exponentialAttenuation;
}

void PositionalLight::setConstantAttenuation(GLfloat constantAttenuation) {

	_constantAttenuation = constantAttenuation;
}

void PositionalLight::setLinearAttenuation(GLfloat linearAttenuation) {

	_linearAttenuation = linearAttenuation;
}

void PositionalLight::setExponentialAttenuation(GLfloat exponentialAttenuation) {

	_exponentialAttenuation = exponentialAttenuation;
}

void PositionalLight::dump() {

	/* Light Dump */
	Light::dump();

	/* PositionalLight Attenuation */
	cout << "<PositionalLight Constant Attenuation> = " << _constantAttenuation << endl;
	cout << "<PositionalLight Linear Attenuation> = " << _linearAttenuation << endl;
	cout << "<PositionalLight Exponential Attenuation> = " << _exponentialAttenuation << endl;
}
