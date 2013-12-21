#include "LightingManager.h"

LightingManager* LightingManager::instance = NULL;

LightingManager::LightingManager() {
}

LightingManager::~LightingManager() {

	map<string,SpotLight*>::const_iterator spotLightIterator;
	for(spotLightIterator = _spotLightMap.begin(); spotLightIterator != _spotLightMap.end(); spotLightIterator++)
		delete spotLightIterator->second;

	map<string,PositionalLight*>::const_iterator positionalLightIterator;
	for(positionalLightIterator = _positionalLightMap.begin(); positionalLightIterator != _positionalLightMap.end(); positionalLightIterator++)
		delete positionalLightIterator->second;

	map<string,DirectionalLight*>::const_iterator directionalLightIterator;
	for(directionalLightIterator = _directionalLightMap.begin(); directionalLightIterator != _directionalLightMap.end(); directionalLightIterator++)
		delete directionalLightIterator->second;
}

LightingManager* LightingManager::getInstance() {

	if(instance == NULL)
		instance = new LightingManager();

	return instance;
}

void LightingManager::destroyInstance() {

	delete instance;

	instance = NULL;
}

void LightingManager::loadUniforms() {

	map<string,SpotLight*>::const_iterator spotLightIterator;
	for(spotLightIterator = _spotLightMap.begin(); spotLightIterator != _spotLightMap.end(); spotLightIterator++)
		loadSpotLight(spotLightIterator->second);

	map<string,PositionalLight*>::const_iterator positionalLightIterator;
	for(positionalLightIterator = _positionalLightMap.begin(); positionalLightIterator != _positionalLightMap.end(); positionalLightIterator++)
		loadPositionalLight(positionalLightIterator->second);

	map<string,DirectionalLight*>::const_iterator directionalLightIterator;
	for(directionalLightIterator = _directionalLightMap.begin(); directionalLightIterator != _directionalLightMap.end(); directionalLightIterator++)
		loadDirectionalLight(directionalLightIterator->second);
}

void LightingManager::loadSpotLight(SpotLight* spotLight) {

	GLint identifier = spotLight->getIdentifier();

	Vector position = spotLight->getPosition();
	Vector direction = spotLight->getDirection();
	Vector color = spotLight->getColor();

	GLfloat lightSourceArray[LIGHT_SOURCE_STRUCT_SIZE];

	/* Position */
	for(int i=0; i<4; i++)
		lightSourceArray[i] = position[i];
	/* Direction */
	for(int i=0; i<4; i++)
		lightSourceArray[i+4] = direction[i];
	/* Color */
	for(int i=0; i<4; i++)
		lightSourceArray[i+8] = color[i];

	/* Cut Off */
	lightSourceArray[12] = spotLight->getCutOff();
	/* Intensity (Ambient, Diffuse, Specular) */
	lightSourceArray[13] = spotLight->getAmbientIntensity();
	lightSourceArray[14] = spotLight->getDiffuseIntensity();
	lightSourceArray[15] = spotLight->getSpecularIntensity();
	/* Attenuation (Constant, Linear, Exponential) */
	lightSourceArray[16] = spotLight->getConstantAttenuation();
	lightSourceArray[17] = spotLight->getLinearAttenuation();
	lightSourceArray[18] = spotLight->getExponentinalAttenuation();

	GLint lightType = SPOT_LIGHT;

	/* Uniform Locations */
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferIndex);

	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * identifier, 
										sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE, lightSourceArray);
	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * identifier + sizeof(GLint) * LIGHT_SOURCE_STRUCT_SIZE, 
										sizeof(GLint), &lightType);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Utility::checkOpenGLError("ERROR: SpotLight \"" + spotLight->getName() + "\" Uniform loading failed.");
}

void LightingManager::loadPositionalLight(PositionalLight* positionalLight) {

	GLint identifier = positionalLight->getIdentifier();

	Vector position = positionalLight->getPosition();
	Vector color = positionalLight->getColor();

	GLfloat lightSourceArray[LIGHT_SOURCE_STRUCT_SIZE];

	/* Position */
	for(int i=0; i<4; i++)
		lightSourceArray[i] = position[i];
	/* Direction */
	for(int i=0; i<4; i++)
		lightSourceArray[i+4] = 0.0f;
	/* Color */
	for(int i=0; i<4; i++)
		lightSourceArray[i+8] = color[i];

	/* Cut Off */
	lightSourceArray[12] = 0.0f;
	/* Intensity (Ambient, Diffuse, Specular) */
	lightSourceArray[13] = positionalLight->getAmbientIntensity();
	lightSourceArray[14] = positionalLight->getDiffuseIntensity();
	lightSourceArray[15] = positionalLight->getSpecularIntensity();
	/* Attenuation (Constant, Linear, Exponential) */
	lightSourceArray[16] = positionalLight->getConstantAttenuation();
	lightSourceArray[17] = positionalLight->getLinearAttenuation();
	lightSourceArray[18] = positionalLight->getExponentinalAttenuation();

	GLint lightType = POSITIONAL_LIGHT;

	/* Uniform Locations */
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferIndex);

	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * identifier, 
										sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE, lightSourceArray);
	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * identifier + sizeof(GLint) * LIGHT_SOURCE_STRUCT_SIZE, 
										sizeof(GLint), &lightType);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Utility::checkOpenGLError("ERROR: PositionalLight \"" + positionalLight->getName() + "\" Uniform loading failed.");
}

void LightingManager::loadDirectionalLight(DirectionalLight* directionalLight) {

	GLint identifier = directionalLight->getIdentifier();

	Vector direction = directionalLight->getDirection();
	Vector color = directionalLight->getColor();

	GLfloat lightSourceArray[LIGHT_SOURCE_STRUCT_SIZE];

	/* Position */
	for(int i=0; i<4; i++)
		lightSourceArray[i] = 0.0f;
	/* Direction */
	for(int i=0; i<4; i++)
		lightSourceArray[i+4] = direction[i];
	/* Color */
	for(int i=0; i<4; i++)
		lightSourceArray[i+8] = color[i];

	/* Cut Off */
	lightSourceArray[12] = 0.0f;
	/* Intensity (Ambient, Diffuse, Specular) */
	lightSourceArray[13] = directionalLight->getAmbientIntensity();
	lightSourceArray[14] = directionalLight->getDiffuseIntensity();
	lightSourceArray[15] = directionalLight->getSpecularIntensity();
	/* Attenuation (Constant, Linear, Exponential) */
	lightSourceArray[16] = 0.0f;
	lightSourceArray[17] = 0.0f;
	lightSourceArray[18] = 0.0f;

	GLint lightType = DIRECTIONAL_LIGHT;

	/* Uniform Locations */
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferIndex);

	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * identifier, 
										sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE, lightSourceArray);
	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * identifier + sizeof(GLint) * LIGHT_SOURCE_STRUCT_SIZE, 
										sizeof(GLint), &lightType);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Utility::checkOpenGLError("ERROR: DirectionalLight \"" + directionalLight->getName() + "\" Uniform loading failed.");
}

void LightingManager::addSpotLight(SpotLight* spotLight) {

	_spotLightMap[spotLight->getName()] = spotLight;
}

void LightingManager::removeSpotLight(string spotLightName) {

	_spotLightMap.erase(spotLightName);
}

SpotLight* LightingManager::getSpotLight(string spotLightName) {

	return _spotLightMap[spotLightName];
}

void LightingManager::addPositionalLight(PositionalLight* positionalLight) {

	_positionalLightMap[positionalLight->getName()] = positionalLight;
}

void LightingManager::removePositionalLight(string positionalLightName) {

	_positionalLightMap.erase(positionalLightName);
}

PositionalLight* LightingManager::getPositionalLight(string positionalLightName) {

	return _positionalLightMap[positionalLightName];
}

void LightingManager::addDirectionalLight(DirectionalLight* directionalLight) {

	_directionalLightMap[directionalLight->getName()] = directionalLight;
}

void LightingManager::removeDirectionalLight(string directionalLightName) {

	_directionalLightMap.erase(directionalLightName);
}

DirectionalLight* LightingManager::getDirectionalLight(string directionalLightName) {

	return _directionalLightMap[directionalLightName];
}

GLuint LightingManager::getUniformBufferIndex() {

	return _uniformBufferIndex;
}

void LightingManager::setUniformBufferIndex(GLuint uniformBufferIndex) {

	_uniformBufferIndex = uniformBufferIndex;
}

void LightingManager::dump() {

	cout << "<LightingManager Dump>" << endl;

	/* Active Camera*/
	cout << "<LightingManager Uniform Buffer Index > = " << _uniformBufferIndex << endl;

	/* Spot Light Map */
	cout << "<LightingManager Scene Node List> = " << endl;
	map<string,SpotLight*>::const_iterator spotLightIterator;
	for(spotLightIterator = _spotLightMap.begin(); spotLightIterator != _spotLightMap.end(); spotLightIterator++)
		spotLightIterator->second->dump();

	/* Positional Light Map */
	cout << "<LightingManager Positional Light Map> = " << endl;
	map<string,PositionalLight*>::const_iterator positionalLightIterator;
	for(positionalLightIterator = _positionalLightMap.begin(); positionalLightIterator != _positionalLightMap.end(); positionalLightIterator++)
		positionalLightIterator->second->dump();

	/* Directional Light Map */
	cout << "<LightingManager Directional Light Map> = " << endl;
	map<string,DirectionalLight*>::const_iterator directionalLightIterator;
	for(directionalLightIterator = _directionalLightMap.begin(); directionalLightIterator != _directionalLightMap.end(); directionalLightIterator++)
		directionalLightIterator->second->dump();
}