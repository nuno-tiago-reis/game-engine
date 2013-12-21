#include "PositionalLight.h"

PositionalLight::PositionalLight(const char* name) : Light(name) {

	_constantAttenuation = 0.0f;
	_linearAttenuation = 0.0f;
	_exponentialAttenuation = 0.0f;
}

PositionalLight::~PositionalLight() {
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
