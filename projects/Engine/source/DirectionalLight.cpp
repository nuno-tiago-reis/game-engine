#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(string name) : Light(name) {
}

DirectionalLight::~DirectionalLight() {
}

void DirectionalLight::loadUniforms() {

	GLfloat lightSourceArray[LIGHT_SOURCE_STRUCT_SIZE];

	/* Position */
	for(int i=0; i<4; i++)
		lightSourceArray[i] = 0.0f;
	/* Direction */
	for(int i=0; i<4; i++)
		lightSourceArray[i+4] = _direction[i];
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
	lightSourceArray[16] = 0.0f;
	lightSourceArray[17] = 0.0f;
	lightSourceArray[18] = 0.0f;

	GLint lightType = DIRECTIONAL_LIGHT;

	/* Uniform Locations */
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferIndex);

	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * _identifier, 
										sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE, lightSourceArray);
	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * _identifier + sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE, 
										sizeof(GLint), &lightType);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Utility::checkOpenGLError("ERROR: DirectionalLight \"" + _name + "\" Uniform loading failed.");
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
