#include "SceneManager.h"

SceneManager* SceneManager::instance = NULL;

SceneManager::SceneManager() {

	_rotationAxis = 0;

	_malletPicked = false;
	_malletDepth = 0.0f;

	_timeCounter = 0.0f;

	_activeCamera = NULL;
	_activeShaderProgram = NULL;

	/* FMOD Sound System Initialization */
	FMOD::System_Create(&_fmodSystem);

	_fmodSystem->init(MAX_SOUND_CHANNELS,FMOD_INIT_NORMAL,0);
}

SceneManager::~SceneManager() {

	/* Destroy Scene Objects */
	map<string,SceneNode*>::const_iterator sceneNodeIterator;
	for(sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		delete sceneNodeIterator->second;

	/* Destroy Shaders */
	map<string,ShaderProgram*>::const_iterator shaderProgramIterator;
	for(shaderProgramIterator = _shaderProgramMap.begin(); shaderProgramIterator != _shaderProgramMap.end(); shaderProgramIterator++)
		delete shaderProgramIterator->second;

	/* Destroy Camera */
	map<string,Camera*>::const_iterator cameraIterator;
	for(cameraIterator = _cameraMap.begin(); cameraIterator != _cameraMap.end(); cameraIterator++)
		delete cameraIterator->second;

	/* Destroy Light */
	map<string,Light*>::const_iterator lightIterator;
	for(lightIterator = _lightMap.begin(); lightIterator != _lightMap.end(); lightIterator++)
		delete lightIterator->second;

	/* Destroy Sound */
	map<string,Sound*>::const_iterator soundIterator;
	for(soundIterator = _soundMap.begin(); soundIterator != _soundMap.end(); soundIterator++)
		delete soundIterator->second;

	/* Sound System Shutdown */
	_fmodSystem->close();
	_fmodSystem->release();

	/* Destroy Matrix Stack */
	MatrixStack::destroyInstance();
	
	/* Destroy Perlin Noise Generator */
	PerlinNoise::destroyInstance();

	/* Destroy User Interaction Handlers */
	MouseHandler::destroyInstance();
	KeyboardHandler::destroyInstance();
}

SceneManager* SceneManager::getInstance() {

	if(instance == NULL)
		instance = new SceneManager();

	return instance;
}

void SceneManager::destroyInstance() {

	delete instance;

	instance = NULL;
}

void SceneManager::init() {

	loadUniforms();

	/* Load Sounds */
	map<string,Sound*>::const_iterator soundIterator;
	for(soundIterator = _soundMap.begin(); soundIterator != _soundMap.end(); soundIterator++)
		soundIterator->second->createSound(_fmodSystem);
}

void SceneManager::loadUniforms() {

	if(_activeShaderProgram == NULL) {

		cerr << "Active Shader Program not initialized." << endl;
		return;
	}

	if(_activeCamera == NULL) {
	
		cerr << "Camera not initialized." << endl;
		return;
	}

	/* Load Light Uniforms */
	map<string,Light*>::const_iterator lightIterator;
	for(lightIterator = _lightMap.begin(); lightIterator != _lightMap.end(); lightIterator++) {

		lightIterator->second->setUniformBufferIndex(_activeShaderProgram->getUniformBufferIndex(LIGHT_SOURCES_UNIFORM));
		lightIterator->second->loadUniforms();
	}

	/* Load Camera Uniforms */
	map<string,Camera*>::const_iterator cameraIterator;
	for(cameraIterator = _cameraMap.begin(); cameraIterator != _cameraMap.end(); cameraIterator++)
		cameraIterator->second->setUniformBufferIndex(_activeShaderProgram->getUniformBufferIndex(MATRICES_UNIFORM));

	_activeCamera->loadUniforms();
}

void SceneManager::draw() {

	map<string,SceneNode*>::const_iterator sceneNodeIterator;

	for(sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->draw(_activeShaderProgram);
}

void SceneManager::rotateJoint(Object* object, Vector rotationDelta, GLfloat elapsedTime) {

	Vector rotation = object->getRotation();
	
	object->setRotation(rotation + rotationDelta * elapsedTime);
}

void SceneManager::update(GLfloat elapsedTime) {

	/* Physics - START */

	/* Physics - END */

	/*_timeCounter += elapsedTime;

	if(_timeCounter > 10.0f)
		_timeCounter = 7.5f;

	if(_timeCounter > 7.5f && _timeCounter < 8.75f) {

		rotateJoint(_objectMap[DRAGON_HEAD],	Vector(0.0f,0.0f,30.0f,1.0f), -elapsedTime);

		rotateJoint(_objectMap[DRAGON_LEFT_WING],	Vector(0.0f,15.0f,0.0f,1.0f), -elapsedTime);
		rotateJoint(_objectMap[DRAGON_LEFT_WING_2],	Vector(0.0f,45.0f,0.0f,1.0f), -elapsedTime);

		rotateJoint(_objectMap[DRAGON_RIGHT_WING],	Vector(0.0f,15.0f,0.0f,1.0f), elapsedTime);
		rotateJoint(_objectMap[DRAGON_RIGHT_WING_2],Vector(0.0f,45.0f,0.0f,1.0f), elapsedTime);

		rotateJoint(_objectMap[DRAGON_TAIL],	Vector(15.0f,0.0f,0.0f,1.0f), -elapsedTime);
		rotateJoint(_objectMap[DRAGON_TAIL_2],	Vector(15.0f,0.0f,0.0f,1.0f), -elapsedTime);
		rotateJoint(_objectMap[DRAGON_TAIL_3],	Vector(15.0f,0.0f,0.0f,1.0f), -elapsedTime);
	}
	else if(_timeCounter > 8.75f && _timeCounter < 10.0f) {

		rotateJoint(_objectMap[DRAGON_HEAD],	Vector(0.0f,0.0f,30.0f,1.0f), elapsedTime);

		rotateJoint(_objectMap[DRAGON_LEFT_WING],	Vector(0.0f,15.0f,0.0f,1.0f), elapsedTime);
		rotateJoint(_objectMap[DRAGON_LEFT_WING_2],	Vector(0.0f,45.0f,0.0f,1.0f), elapsedTime);

		rotateJoint(_objectMap[DRAGON_RIGHT_WING],	Vector(0.0f,15.0f,0.0f,1.0f), -elapsedTime);
		rotateJoint(_objectMap[DRAGON_RIGHT_WING_2],Vector(0.0f,45.0f,0.0f,1.0f), -elapsedTime);

		rotateJoint(_objectMap[DRAGON_TAIL],	Vector(15.0f,0.0f,0.0f,1.0f), elapsedTime);
		rotateJoint(_objectMap[DRAGON_TAIL_2],	Vector(15.0f,0.0f,0.0f,1.0f), elapsedTime);
		rotateJoint(_objectMap[DRAGON_TAIL_3],	Vector(15.0f,0.0f,0.0f,1.0f), elapsedTime);
	}*(

	/* Dragon Movement */
	/*Vector dragonRotation = _objectMap[DRAGON_BODY]->getRotation();
	dragonRotation[VZ] += 0.75f;

	_objectMap[DRAGON_BODY]->setRotation(dragonRotation);

	Vector dragonPosition = _objectMap[DRAGON_BODY]->getPosition();
	
	dragonPosition[VX] += cos((dragonRotation[VZ] + 90) * DEGREES_TO_RADIANS) * 0.075f;
	dragonPosition[VY] += sin((dragonRotation[VZ] + 90) * DEGREES_TO_RADIANS) * 0.075f;

	_objectMap[DRAGON_BODY]->setPosition(dragonPosition);
	*/
	
	/* Scene Update */
	readMouse(elapsedTime);
	readKeyboard(elapsedTime);

	Vector cubeRotation = _objectMap[BUMP_MAPPING_CUBE]->getRotation();

	switch(_rotationAxis) {
	
		case 0:	break;

		case 1:	cubeRotation[VX] += 50.00f * elapsedTime;
				break;
		case 2:	cubeRotation[VY] += 50.00f * elapsedTime;
				break;
		case 3:	cubeRotation[VZ] += 50.00f * elapsedTime;
				break;
	}

	_objectMap[BUMP_MAPPING_CUBE]->setRotation(cubeRotation);

	_fmodSystem->update();

	/* Update Scene Graph */
	map<string,SceneNode*>::const_iterator sceneNodeIterator;
	for(sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->update(elapsedTime);
}

void SceneManager::reshape(GLint width, GLint height) {

	map<string,Camera*>::const_iterator cameraIterator;

	for(cameraIterator = _cameraMap.begin(); cameraIterator != _cameraMap.end(); cameraIterator++)
		cameraIterator->second->reshape(width,height);

	loadUniforms();
}

void SceneManager::readKeyboard(GLfloat elapsedTime) {
	
	KeyboardHandler* handler = KeyboardHandler::getInstance();

	if(!handler->isKeyboardEnabled())
		return;	

	handler->disableKeyboard();

	if(handler->isSpecialKeyPressed(GLUT_KEY_UP)) {

		if(handler->wasSpecialKeyPressed(GLUT_KEY_UP) == true) {

			Vector position = _lightMap["Positional Light 1"]->getPosition();

			position[VZ] += 0.1f;

			_lightMap["Positional Light 1"]->setPosition(position);
			_lightMap["Positional Light 1"]->loadUniforms();
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_DOWN)) {

		if(handler->wasSpecialKeyPressed(GLUT_KEY_DOWN) == true) {

			Vector position = _lightMap["Positional Light 1"]->getPosition();

			position[VZ] -= 0.1f;

			_lightMap["Positional Light 1"]->setPosition(position);
			_lightMap["Positional Light 1"]->loadUniforms();
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_RIGHT)) {

		if(handler->wasSpecialKeyPressed(GLUT_KEY_RIGHT) == true) {

			Vector position = _lightMap["Spot Light 2"]->getPosition();

			position[VZ] += 0.1f;

			_lightMap["Spot Light 2"]->setPosition(position);
			_lightMap["Spot Light 2"]->loadUniforms();
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_LEFT)) {

		if(handler->wasSpecialKeyPressed(GLUT_KEY_LEFT) == true) {

			Vector position = _lightMap["Spot Light 2"]->getPosition();

			position[VZ] -= 0.1f;

			_lightMap["Spot Light 2"]->setPosition(position);
			_lightMap["Spot Light 2"]->loadUniforms();
		}
	}

	/* Sound Buttons */
	if(handler->isSpecialKeyPressed(GLUT_KEY_F1)) {
		
		/* Edit Mode - Increment Up */
		if(handler->wasSpecialKeyPressed(GLUT_KEY_F1) == false) {

			
			_fmodSystem->playSound(FMOD_CHANNEL_FREE, _soundMap[RAYQUAZA_SOUND_NAME]->getFmodSound(), false, &channel[0]);
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_F2)) {
		
		/* Edit Mode - Increment Up */
		if(handler->wasSpecialKeyPressed(GLUT_KEY_F2) == false) {

			_fmodSystem->playSound(FMOD_CHANNEL_FREE, _soundMap[MUSIC_SOUND_NAME]->getFmodSound(), false, &channel[1]);
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_F3)) {
		
		/* Edit Mode - Increment Up */
		if(handler->wasSpecialKeyPressed(GLUT_KEY_F3) == false) {

			bool paused;

			channel[1]->getPaused(&paused);

			if(paused)
				channel[1]->setPaused(false);
			else
				channel[1]->setPaused(true);
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_F4)) {
		
		/* Edit Mode - Increment Up */
		if(handler->wasSpecialKeyPressed(GLUT_KEY_F4) == false) {

			channel[1]->stop();
		}
	}	

	if(handler->isKeyPressed('x'))
		if(!handler->wasKeyPressed('x'))
			_rotationAxis = 1;

	if(handler->isKeyPressed('y'))
		if(!handler->wasKeyPressed('y'))
			_rotationAxis = 2;

	if(handler->isKeyPressed('z'))
		if(!handler->wasKeyPressed('z'))
			_rotationAxis = 3;

	if(handler->isKeyPressed('s'))
		if(!handler->wasKeyPressed('s'))
			_rotationAxis = 0;

	if(handler->wasKeyPressedThisFrame('q'))
		exit(0);
	
	handler->enableKeyboard();
}

void SceneManager::readMouse(GLfloat elapsedTime) {

	MouseHandler* handler = MouseHandler::getInstance();

	if(!handler->isMouseEnabled())
		return;	

	handler->disableMouse();

	GLint zoom = handler->getMouseWheelPosition();
	GLint longitude = handler->getLongitude(GLUT_RIGHT_BUTTON);
	GLint latitude = handler->getLatitude(GLUT_RIGHT_BUTTON);

	_activeCamera->update(zoom,longitude,latitude,elapsedTime);

	if(handler->isButtonPressed(GLUT_LEFT_BUTTON)) {

		GLint* mousePosition = handler->getMouseClickPosition(GLUT_LEFT_BUTTON);

		//rayCast(mousePosition,elapsedTime);

		delete mousePosition;

	}
	else if(handler->wasButtonPressed(GLUT_LEFT_BUTTON)) {

		_malletPicked = false;
	}

	handler->enableMouse();
}

void SceneManager::rayCast(GLint* mousePosition, GLfloat elapsedTime) {

	Matrix invertedProjectionMatrix = _activeCamera->getProjectionMatrix();
	invertedProjectionMatrix.invert();
	invertedProjectionMatrix.transpose();

	Matrix invertedViewMatrix = _activeCamera->getViewMatrix();
	invertedViewMatrix.invert();
	invertedViewMatrix.transpose();

	Vector rayOrigin;
	Vector rayTarget;

	if(_activeCamera->getProjectionMode() == PERSPECTIVE) {
	
		rayOrigin = _activeCamera->getEye();

		rayTarget[VX] = 2.0f * mousePosition[0] / _activeCamera->getWidth() - 1.0f;
		rayTarget[VY] = 1.0f - (2.0f * mousePosition[1]) / _activeCamera->getHeight();
		rayTarget[VZ] = 1.0f;
		rayTarget[VW] = 1.0f;

		rayTarget = invertedProjectionMatrix * rayTarget;

		rayTarget[VW] = 1.0f;

		rayOrigin.clean();
		rayTarget.clean();

		rayTarget = invertedViewMatrix * rayTarget;

		rayTarget.clean();
	}
	else {

		rayOrigin[VX] = 2.0f * mousePosition[0] / _activeCamera->getWidth() - 1.0f;
		rayOrigin[VY] = 1.0f - (2.0f * mousePosition[1]) / _activeCamera->getHeight();
		rayOrigin[VZ] =-1.0f;
		rayOrigin[VW] = 1.0f;

		rayTarget[VX] = 2.0f * mousePosition[0] / _activeCamera->getWidth() - 1.0f;
		rayTarget[VY] = 1.0f - (2.0f * mousePosition[1]) / _activeCamera->getHeight();
		rayTarget[VZ] = 1.0f;
		rayTarget[VW] = 1.0f;

		rayOrigin = invertedProjectionMatrix * rayOrigin;
		rayTarget = invertedProjectionMatrix * rayTarget;

		rayOrigin.clean();
		rayTarget.clean();

		rayOrigin = invertedViewMatrix * rayOrigin;
		rayTarget = invertedViewMatrix * rayTarget;

		rayOrigin.clean();
		rayTarget.clean();
	}

	Vector rayDirection = rayTarget;
	rayDirection -= rayOrigin;
	rayDirection.normalize();
	rayDirection.clean();

	rayOrigin.clean();

	Object* mallet = _objectMap[MALLET];
	Object* platform = _objectMap[PLATFORM];

	GLfloat malletIntersectionPoint = mallet->isIntersecting(rayOrigin,rayDirection);
	GLfloat platformIntersectionPoint = platform->isIntersecting(rayOrigin,rayDirection);

	if(malletIntersectionPoint == NULL && platformIntersectionPoint == NULL)
		_malletPicked = false;

	if(malletIntersectionPoint != NULL)
		_malletPicked = true;
				
	if(platformIntersectionPoint != NULL && _malletPicked == true) {

		_malletDepth = platformIntersectionPoint;

		Vector nextPosition = rayOrigin + rayDirection * _malletDepth;
		Vector lastPosition = mallet->getPosition();

		nextPosition[VZ] = 0.0f;

		Vector velocity = nextPosition - lastPosition;

		velocity = velocity * (1.0f / elapsedTime);
		velocity[VW] = 1.0f;

		mallet->setVelocity(velocity);
	}

	/*if(platformIntersectionPoint != NULL)
		cout << "Intersected " << platform->getName() << " " << platformIntersectionPoint << " " << rand()%100 << endl;*/
}

void SceneManager::setActiveCamera(Camera* camera) {

	_activeCamera = camera;
}

Camera* SceneManager::getActiveCamera() {

	return _activeCamera;
}

void SceneManager::addCamera(Camera* camera) {

	_cameraMap[camera->getName()] = camera;
}

void SceneManager::removeCamera(string cameraName) {

	_cameraMap.erase(cameraName);
}

Camera* SceneManager::getCamera(string cameraName) {

	return _cameraMap[cameraName];
}

void SceneManager::setActiveShaderProgram(ShaderProgram* shaderProgram) {

	_activeShaderProgram = shaderProgram;
}

ShaderProgram* SceneManager::getActiveShaderProgram() {

	return _activeShaderProgram;
}

void SceneManager::addSound(Sound* sound) {

	_soundMap[sound->getName()] = sound;
}

void SceneManager::removeSound(string soundName) {

	_soundMap.erase(soundName);
}

Sound* SceneManager::getSound(string soundName) {

	return _soundMap[soundName];
}

void SceneManager::addLight(Light* light) {

	_lightMap[light->getName()] = light;
}

void SceneManager::removeLight(string lightName) {

	_lightMap.erase(lightName);
}

Light* SceneManager::getLight(string lightName) {

	return _lightMap[lightName];
}

void SceneManager::addShaderProgram(ShaderProgram* shaderProgram) {

	_shaderProgramMap[shaderProgram->getName()] = shaderProgram;
}

void SceneManager::removeShaderProgram(string shaderProgramName) {

	_shaderProgramMap.erase(shaderProgramName);
}

ShaderProgram* SceneManager::getShaderProgram(string shaderProgramName) {

	return _shaderProgramMap[shaderProgramName];
}

void SceneManager::addObject(Object* graphicObject) {

	_objectMap[graphicObject->getName()] = graphicObject;
}

void SceneManager::removeObject(string graphicObjectName) {

	_objectMap.erase(graphicObjectName);
}

Object* SceneManager::getObject(string graphicObjectName) {

	return _objectMap[graphicObjectName];
}

void SceneManager::addSceneNode(SceneNode* sceneNode) {

	_sceneNodeMap[sceneNode->getName()] = sceneNode;
}

void SceneManager::removeSceneNode(string sceneNodeName) {

	_sceneNodeMap.erase(sceneNodeName);
}

SceneNode* SceneManager::getSceneNode(string sceneNodeName) {

	return _sceneNodeMap[sceneNodeName];
}

void SceneManager::dump() {

	cout << "<SceneManager Dump>" << endl;

	/* Active Camera*/
	cout << "<SceneManager Active Camera> = " << endl;
	_activeCamera->dump(); 

	/* Active Shader Program */
	cout << "<SceneManager Active Shader Program> = " << endl;
	_activeShaderProgram->dump(); 

	/* Sound Map */
	cout << "<SceneManager Sound List> = " << endl;
	for(map<string,Sound*>::const_iterator soundMap = _soundMap.begin(); soundMap != _soundMap.end(); soundMap++)
		soundMap->second->dump();

	/* Light Map */
	cout << "<SceneManager Light List> = " << endl;
	for(map<string,Light*>::const_iterator lightIterator = _lightMap.begin(); lightIterator != _lightMap.end(); lightIterator++)
		lightIterator->second->dump();

	/* Camera Map */
	cout << "<SceneManager Camera List> = " << endl;
	for(map<string,Camera*>::const_iterator cameraIterator = _cameraMap.begin(); cameraIterator != _cameraMap.end(); cameraIterator++)
		cameraIterator->second->dump();

	/* Shader Program Map */
	cout << "<SceneManager Shader List> = " << endl;
	for(map<string,ShaderProgram*>::const_iterator shaderProgramIterator = _shaderProgramMap.begin(); shaderProgramIterator != _shaderProgramMap.end(); shaderProgramIterator++)
		shaderProgramIterator->second->dump();

	/* Graphic Object Map */
	cout << "<SceneManager Object List> = " << endl;
	for(map<string,Object*>::const_iterator graphicObjectIterator = _objectMap.begin(); graphicObjectIterator != _objectMap.end(); graphicObjectIterator++)
		graphicObjectIterator->second->dump();

	/* Scene Node Map */
	cout << "<SceneManager Scene Node List> = " << endl;
	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->dump();
}