#include "SceneManager.h"

SceneManager* SceneManager::instance = NULL;

SceneManager::SceneManager() {

	_malletPicked = false;
	_malletDepth = 0.0f;

	_timeCounter = 0.0f;

	_activeCamera = NULL;
	_activeShaderProgram = NULL;
}

SceneManager::~SceneManager() {

	map<string,Camera*>::const_iterator cameraIterator;

	for(cameraIterator = _cameraMap.begin(); cameraIterator != _cameraMap.end(); cameraIterator++) {

		Camera* camera = cameraIterator->second;
		
		delete camera;
	}

	map<string,ShaderProgram*>::const_iterator shaderProgramIterator;

	for(shaderProgramIterator = _shaderProgramMap.begin(); shaderProgramIterator != _shaderProgramMap.end(); shaderProgramIterator++) {

		ShaderProgram* shaderProgram = shaderProgramIterator->second;

		shaderProgram->destroyShaderProgram();
		
		delete shaderProgram;
	}

	map<string,GraphicObject*>::const_iterator graphicObjectIterator;

	for(graphicObjectIterator = _graphicObjectMap.begin(); graphicObjectIterator != _graphicObjectMap.end(); graphicObjectIterator++) {

		GraphicObject* graphicObject = graphicObjectIterator->second;
		
		delete graphicObject;
	}

	MatrixStack::destroyInstance();
	CollisionTest::destroyInstance();
	LightingManager::destroyInstance();

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

void SceneManager::loadUniforms() {

	if(_activeShaderProgram == NULL) {

		cerr << "Active Shader Program not initialized." << endl;
		return;
	}

	if(_activeCamera == NULL) {
	
		cerr << "Camera not initialized." << endl;
		return;
	}

	/* Init Lighting Manager */
	LightingManager* lightingManager = LightingManager::getInstance();

	lightingManager->setUniformBufferIndex(_activeShaderProgram->getUniformBufferIndex(LIGHT_SOURCES_UNIFORM));
	lightingManager->loadUniforms();

	/* Init Active Camera */
	_activeCamera->setUniformBufferIndex(_activeShaderProgram->getUniformBufferIndex(MATRICES_UNIFORM));
	_activeCamera->loadUniforms();
}

void SceneManager::draw() {

	map<string,SceneNode*>::const_iterator sceneNodeIterator;

	for(sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->draw(_activeShaderProgram);
}

void SceneManager::update(GLfloat elapsedTime) {

	_timeCounter += elapsedTime;

	if(_timeCounter > 3.0f)
		_timeCounter = 0.0f;

	if(_timeCounter > 1.5f) {

		Vector rotation = _graphicObjectMap[DRAGON_LEFT_ARM]->getRotation();
	
		rotation[VY]--;

		_graphicObjectMap[DRAGON_LEFT_ARM]->setRotation(rotation);

		rotation = _graphicObjectMap[DRAGON_RIGHT_ARM]->getRotation();
	
		rotation[VY]++;

		_graphicObjectMap[DRAGON_RIGHT_ARM]->setRotation(rotation);

		rotation = _graphicObjectMap[DRAGON_TAIL]->getRotation();
	
		rotation[VX]++;

		_graphicObjectMap[DRAGON_TAIL]->setRotation(rotation);

		rotation = _graphicObjectMap[DRAGON_TAIL_2]->getRotation();
	
		rotation[VZ]++;

		_graphicObjectMap[DRAGON_TAIL_2]->setRotation(rotation);
	}
	else {
	
		Vector rotation = _graphicObjectMap[DRAGON_LEFT_ARM]->getRotation();
	
		rotation[VY]++;

		_graphicObjectMap[DRAGON_LEFT_ARM]->setRotation(rotation);

		rotation = _graphicObjectMap[DRAGON_RIGHT_ARM]->getRotation();
	
		rotation[VY]--;

		_graphicObjectMap[DRAGON_RIGHT_ARM]->setRotation(rotation);

		rotation = _graphicObjectMap[DRAGON_TAIL]->getRotation();
	
		rotation[VX]--;

		_graphicObjectMap[DRAGON_TAIL]->setRotation(rotation);

		rotation = _graphicObjectMap[DRAGON_TAIL_2]->getRotation();
	
		rotation[VZ]--;

		_graphicObjectMap[DRAGON_TAIL_2]->setRotation(rotation);
	}

	/* Dragon Movement */
	Vector dragonRotation = _graphicObjectMap[DRAGON_BODY]->getRotation();
	
	dragonRotation[VZ] += 0.75f;

	_graphicObjectMap[DRAGON_BODY]->setRotation(dragonRotation);

	Vector dragonPosition = _graphicObjectMap[DRAGON_BODY]->getPosition();
	
	dragonPosition[VX] += cos((dragonRotation[VZ] + 90) * DEGREES_TO_RADIANS) * 0.05f;
	dragonPosition[VY] += sin((dragonRotation[VZ] + 90) * DEGREES_TO_RADIANS) * 0.05f;

	_graphicObjectMap[DRAGON_BODY]->setPosition(dragonPosition);
	
	/* Scene Update */
	readMouse(elapsedTime);
	readKeyboard(elapsedTime);

	map<string,SceneNode*>::const_iterator sceneNodeIterator;
	for(sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->updateBoundingVolume(elapsedTime);

	testCollision(_graphicObjectMap[MALLET],_graphicObjectMap[PUCK]);

	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[PIN_A]);
	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[PIN_B]);
	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[PIN_C]);		

	testCollision(_graphicObjectMap[MALLET],_graphicObjectMap[WALL_A]);
	testCollision(_graphicObjectMap[MALLET],_graphicObjectMap[WALL_B]);
	testCollision(_graphicObjectMap[MALLET],_graphicObjectMap[WALL_C]);
	testCollision(_graphicObjectMap[MALLET],_graphicObjectMap[WALL_D]);

	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[WALL_A]);	
	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[WALL_B]);
	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[WALL_C]);
	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[WALL_D]);

	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[WALL_A]);	
	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[WALL_B]);
	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[WALL_C]);
	testCollision(_graphicObjectMap[PUCK],_graphicObjectMap[WALL_D]);

	for(sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->update(elapsedTime);

	Vector position = _graphicObjectMap[MALLET]->getPosition();
	Vector velocity = _graphicObjectMap[MALLET]->getVelocity();
	GLfloat radius = ((BoundingSphere*)_graphicObjectMap[MALLET]->getBoundingVolume())->getRadius();

	if(position[VX] >= 3.50f - radius) {

		position[VX] = 3.50f - radius;
		velocity[VX] = 0.0f;
	}
	else if(position[VX] <= -3.50f + radius) {

		position[VX] = -3.50f + radius;
		velocity[VX] = 0.0f;
	}

	if(position[VY] >= 5.75f - radius) {

		position[VY] = 5.75f - radius;
		velocity[VY] = 0.0f;
	}
	else if(position[VY] <= -5.75f + radius) {

		position[VY] = -5.75f + radius;
		velocity[VY] = 0.0f;
	}

	_graphicObjectMap[MALLET]->setPosition(position);
	_graphicObjectMap[MALLET]->setVelocity(velocity);

	for(sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->update();
}

void SceneManager::reshape(GLint width, GLint height) {

	map<string,Camera*>::const_iterator cameraIterator;

	for(cameraIterator = _cameraMap.begin(); cameraIterator != _cameraMap.end(); cameraIterator++)
		cameraIterator->second->reshape(width,height);

	_activeCamera->loadUniforms();
}

void SceneManager::testCollision(GraphicObject* graphicObjectA, GraphicObject* graphicObjectB) {

	CollisionTest* instance = CollisionTest::getInstance();

	if(instance->isColliding(graphicObjectA->getBoundingVolume(),graphicObjectB->getBoundingVolume())) {

		graphicObjectA->updateCollision();
		graphicObjectB->updateCollision();
	}
}

void SceneManager::readKeyboard(GLfloat elapsedTime) {
	
	KeyboardHandler* handler = KeyboardHandler::getInstance();

	if(!handler->isKeyboardEnabled())
		return;	

	handler->disableKeyboard();

	/* Reset Button */
	if(handler->isKeyPressed('i')) {
		
		/* Edit Mode - Increment Up */
		if(handler->wasKeyPressed('i')) {

			Vector position(0.0f, -2.5f, 0.0f, 1.0f);
			Vector velocity(0.0f, 0.0f, 0.0f, 1.0f);

			_graphicObjectMap[MALLET]->setPosition(position);
			_graphicObjectMap[MALLET]->setVelocity(velocity);
		}
	}

	/* Camera Control */
	if(handler->isSpecialKeyPressed(GLUT_KEY_F1)) {

		if(handler->wasSpecialKeyPressed(GLUT_KEY_F1)) {

			if(SCENE_GRAPH_TESTING) {

				Vector position = _activeCamera->getPosition();
				position[0] += 5.0f * elapsedTime;

				_activeCamera->setPosition(position);
			}
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_F2)) {

		if(handler->wasSpecialKeyPressed(GLUT_KEY_F2)) {

			if(SCENE_GRAPH_TESTING) {

				Vector position = _activeCamera->getPosition();
				position[0] -= 5.0f * elapsedTime;

				_activeCamera->setPosition(position);
			}
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_F3)) {

		if(handler->wasSpecialKeyPressed(GLUT_KEY_F3)) {

			if(SCENE_GRAPH_TESTING) {

				Vector position = _activeCamera->getPosition();
				position[1] += 5.0f * elapsedTime;

				_activeCamera->setPosition(position);
			}
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_F4)) {

		if(handler->wasSpecialKeyPressed(GLUT_KEY_F4)) {

			if(SCENE_GRAPH_TESTING) {

				Vector position = _activeCamera->getPosition();
				position[1] -= 5.0f * elapsedTime;

				_activeCamera->setPosition(position);
			}
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_F5)) {

		if(handler->wasSpecialKeyPressed(GLUT_KEY_F5)) {

			if(SCENE_GRAPH_TESTING) {

				Vector position = _activeCamera->getPosition();
				position[2] += 5.0f * elapsedTime;

				_activeCamera->setPosition(position);
			}
		}
	}

	if(handler->isSpecialKeyPressed(GLUT_KEY_F6)) {

		if(handler->wasSpecialKeyPressed(GLUT_KEY_F6)) {

			if(SCENE_GRAPH_TESTING) {

				Vector position = _activeCamera->getPosition();
				position[2] -= 5.0f * elapsedTime;

				_activeCamera->setPosition(position);
			}
		}
	}

	/* Scene Settings */
	if(handler->wasKeyPressedThisFrame('c')){

		if(_activeCamera->getName().compare(PERSPECTIVE_NAME) == 0)
			_activeCamera = _cameraMap[ORTHOGONAL_NAME];
		else
			_activeCamera = _cameraMap[PERSPECTIVE_NAME];

		_activeCamera->setUniformBufferIndex(_activeShaderProgram->getUniformBufferIndex(MATRICES_UNIFORM));
		_activeCamera->loadUniforms();
	}	

	if(handler->wasKeyPressedThisFrame('q')){

		exit(1);
	}
	
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

		if(!SCENE_GRAPH_TESTING)
			rayCast(mousePosition,elapsedTime);

		delete mousePosition;

	}
	else if(handler->wasButtonPressed(GLUT_LEFT_BUTTON)) {

		_malletPicked = false;
	}

	handler->enableMouse();
}

void SceneManager::rayCast(GLint* mousePosition, GLfloat elapsedTime) {

	Matrix invertedProjectionMatrix(_activeCamera->getProjectionMatrix().getValue());
	Matrix invertedViewMatrix(_activeCamera->getViewMatrix().getValue());

	invertedProjectionMatrix.invert();
	invertedViewMatrix.invert();

	invertedProjectionMatrix.transpose();
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

	Vector rayDirection(rayTarget.getValue());

	rayDirection -= rayOrigin;
	rayDirection.normalize();
	rayDirection.clean();

	rayOrigin.clean();

	GraphicObject* mallet = _graphicObjectMap[MALLET];
	GraphicObject* platform = _graphicObjectMap[PLATFORM];

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

void SceneManager::addShaderProgram(ShaderProgram* shaderProgram) {

	_shaderProgramMap[shaderProgram->getName()] = shaderProgram;
}

void SceneManager::removeShaderProgram(string shaderProgramName) {

	_shaderProgramMap.erase(shaderProgramName);
}

ShaderProgram* SceneManager::getShaderProgram(string shaderProgramName) {

	return _shaderProgramMap[shaderProgramName];
}

void SceneManager::addGraphicObject(GraphicObject* graphicObject) {

	_graphicObjectMap[graphicObject->getName()] = graphicObject;
}

void SceneManager::removeGraphicObject(string graphicObjectName) {

	_graphicObjectMap.erase(graphicObjectName);
}

GraphicObject* SceneManager::getGraphicObject(string graphicObjectName) {

	return _graphicObjectMap[graphicObjectName];
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

	/* Camera Map */
	cout << "<SceneManager Camera List> = " << endl;
	for(map<string,Camera*>::const_iterator cameraIterator = _cameraMap.begin(); cameraIterator != _cameraMap.end(); cameraIterator++)
		cameraIterator->second->dump();

	/* Active Shader Program */
	cout << "<SceneManager Active Shader Program> = " << endl;
	_activeShaderProgram->dump(); 

	/* Shader Program Map */
	cout << "<SceneManager Shader List> = " << endl;
	for(map<string,ShaderProgram*>::const_iterator shaderProgramIterator = _shaderProgramMap.begin(); shaderProgramIterator != _shaderProgramMap.end(); shaderProgramIterator++)
		shaderProgramIterator->second->dump();

	/* Scene Node Map */
	cout << "<SceneManager Scene Node List> = " << endl;
	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->dump();

	/* Graphic Object Map */
	cout << "<SceneManager Graphic Object List> = " << endl;
	for(map<string,GraphicObject*>::const_iterator graphicObjectIterator = _graphicObjectMap.begin(); graphicObjectIterator != _graphicObjectMap.end(); graphicObjectIterator++)
		graphicObjectIterator->second->dump();
}