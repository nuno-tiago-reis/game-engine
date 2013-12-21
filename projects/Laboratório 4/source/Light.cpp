#include "Light.h"

Light::Light(const char* name) {

	_name.assign(name);

	_identifier = 0;

	_ambientIntensity = 0.0f;
	_diffuseIntensity = 0.0f;
	_specularIntensity = 0.0f;
}

Light::~Light() {
}

string Light::getName() {
	
	return _name;
}

GLint Light::getIdentifier() {

	return _identifier;
}

Vector Light::getPosition() {

	return _position;
}

Vector Light::getColor() {

	return _color;
}

GLfloat Light::getAmbientIntensity() {

	return _ambientIntensity;
}

GLfloat Light::getDiffuseIntensity() {

	return _diffuseIntensity;
}

GLfloat Light::getSpecularIntensity() {

	return _specularIntensity;
}

void Light::setName(const char* name) {

	_name.assign(name);
}

void Light::setIdentifier(GLint identifier) {

	_identifier = identifier;
}

void Light::setPosition(Vector position) {

	_position = position;
}

void Light::setColor(Vector color) {

	_color = color;
}

void Light::setAmbientIntensity(GLfloat ambientIntensity) {

	_ambientIntensity = ambientIntensity;
}

void Light::setDiffuseIntensity(GLfloat diffuseIntensity) {

	_diffuseIntensity = diffuseIntensity;
}

void Light::setSpecularIntensity(GLfloat specularIntensity) {

	_specularIntensity = specularIntensity;
}

void Light::dump() {

	cout << "<Light \"" << _name << "\" Dump>" << endl;

	/* Light Position */
	cout << "<Light Position> = " << endl;
	_position.dump();

	/* Light Color */
	cout << "<Light Color> = " << endl;
	_color.dump();

	/* Light Intensity */
	cout << "<Light Ambient Intensity> = " << _ambientIntensity << endl;
	cout << "<Light Diffuse Intensity> = " << _diffuseIntensity << endl;
}
