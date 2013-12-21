#include "SpotLight.h"

SpotLight::SpotLight(const char* name) : Light(name) {

	_cutOff = 45.0f;

	_constantAttenuation = 0.0f;
	_linearAttenuation = 0.0f;
	_exponentialAttenuation = 0.0f;
}

SpotLight::~SpotLight() {
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
