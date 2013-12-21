#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const char* name) : Light(name) {
}

DirectionalLight::~DirectionalLight() {
}

Vector DirectionalLight::getDirection() {

	return _direction;
}

void DirectionalLight::setDirection(Vector direction) {

	_direction = direction;
}

void DirectionalLight::dump() {

	/* Light Dump */
	Light::dump();

	/* DirectionalLight Direction */
	cout << "<DirectionalLight Direction> = " << endl;
	_direction.dump();
}
