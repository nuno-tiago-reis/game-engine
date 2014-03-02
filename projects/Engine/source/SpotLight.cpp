#include "SpotLight.h"

SpotLight::SpotLight(string name) : Light(name) {

	_cutOff = 45.0f;

	_constantAttenuation = 0.0f;
	_linearAttenuation = 0.0f;
	_exponentialAttenuation = 0.0f;
}

SpotLight::~SpotLight() {
}

void SpotLight::loadUniforms() {

	GLfloat lightSourceArray[LIGHT_SOURCE_STRUCT_SIZE];

	/* Position */
	for(int i=0; i<4; i++)
		lightSourceArray[i] = _position[i];
	/* Direction */
	for(int i=0; i<4; i++)
		lightSourceArray[i+4] = _direction[i];
	/* Color */
	for(int i=0; i<4; i++)
		lightSourceArray[i+8] = _color[i];

	/* Cut Off */
	lightSourceArray[12] = _cutOff;
	/* Intensity (Ambient, Diffuse, Specular) */
	lightSourceArray[13] = _ambientIntensity;
	lightSourceArray[14] = _diffuseIntensity;
	lightSourceArray[15] = _specularIntensity;
	/* Attenuation (Constant, Linear, Exponential) */
	lightSourceArray[16] = _constantAttenuation;
	lightSourceArray[17] = _linearAttenuation;
	lightSourceArray[18] = _exponentialAttenuation;

	GLint lightType = SPOT_LIGHT;

	/* Uniform Locations */
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferIndex);

	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * _identifier, 
										sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE, lightSourceArray);
	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * _identifier + sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE, 
										sizeof(GLint), &lightType);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Utility::checkOpenGLError("ERROR: SpotLight \"" + _name + "\" Uniform loading failed.");
}

Vector SpotLight::getDirection() {

	return _direction;
}

GLfloat SpotLight::getCutOff() {

	return _cutOff;
}

GLfloat SpotLight::getConstantAttenuation() {

	return _constantAttenuation;
}

GLfloat SpotLight::getLinearAttenuation() {

	return _linearAttenuation;
}

GLfloat SpotLight::getExponentinalAttenuation() {

	return _exponentialAttenuation;
}

void SpotLight::setDirection(Vector direction) {

	_direction = direction;
}

void SpotLight::setCutOff(GLfloat cutOff) {

	_cutOff = cutOff;
}

void SpotLight::setConstantAttenuation(GLfloat constantAttenuation) {

	_constantAttenuation = constantAttenuation;
}

void SpotLight::setLinearAttenuation(GLfloat linearAttenuation) {

	_linearAttenuation = linearAttenuation;
}

void SpotLight::setExponentialAttenuation(GLfloat exponentialAttenuation) {

	_exponentialAttenuation = exponentialAttenuation;
}

void SpotLight::dump() {

	/* Light Dump */
	Light::dump();

	/* SpotLight Direction */
	cout << "<SpotLight Direction> = " << endl;
	_direction.dump();

	/* SpotLight Cut Off */
	cout << "<SpotLight Cut Off> = " << _cutOff << endl;

	/* SpotLight Attenuation */
	cout << "<SpotLight Constant Attenuation> = " << _constantAttenuation << endl;
	cout << "<SpotLight Linear Attenuation> = " << _linearAttenuation << endl;
	cout << "<SpotLight Exponential Attenuation> = " << _exponentialAttenuation << endl;
}
