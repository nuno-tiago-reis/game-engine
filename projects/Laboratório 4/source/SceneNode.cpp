#include "SceneNode.h"

SceneNode::SceneNode(const char* name) {

	_name.assign(name);

	_shaderProgram = NULL;

	_graphicObject = NULL;
}

SceneNode::~SceneNode() {
}

void SceneNode::draw(ShaderProgram* shaderProgram) {

	if(shaderProgram != NULL && _shaderProgram == NULL)
		_shaderProgram = shaderProgram;

	if(_shaderProgram == NULL) {

		cerr << "<Scene Node> Shader not initialized." << endl;
		return;
	}

	_graphicObject->draw(_shaderProgram->getProgramID(),_shaderProgram->getUniformLocation(MATRICES_UNIFORM));

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->draw(_shaderProgram);
}

void SceneNode::update() {

	MatrixStack* matrixStack = MatrixStack::getInstance();

	_graphicObject->setModelMatrix(matrixStack->peek());

	_graphicObject->update();

	matrixStack->push(_graphicObject->getModelMatrix());

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->update();

	matrixStack->pop();
}

void SceneNode::update(GLfloat elapsedTime) {

	MatrixStack* matrixStack = MatrixStack::getInstance();

	_graphicObject->setModelMatrix(matrixStack->peek());

	_graphicObject->update(elapsedTime);

	matrixStack->push(_graphicObject->getModelMatrix());

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->update(elapsedTime);

	matrixStack->pop();
}

void SceneNode::updateBoundingVolume(GLfloat elapsedTime) {

	_graphicObject->updateBoundingVolume(elapsedTime);

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->updateBoundingVolume(elapsedTime);	
}

void SceneNode::addChildNode(SceneNode* sceneNode) {

	sceneNode->getGraphicObject()->setParentName(_graphicObject->getName().c_str());

	_sceneNodeMap[sceneNode->getGraphicObject()->getName()] = sceneNode;
}

bool SceneNode::removeChildNode(string childNodeName) {

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++) {

		GraphicObject* graphicObject = sceneNodeIterator->second->getGraphicObject();

		if(graphicObject->getName().compare(childNodeName) == 0) {

			graphicObject->setParentName("None");

			_sceneNodeMap.erase(childNodeName);

			return true;
		}

		if(sceneNodeIterator->second->removeChildNode(childNodeName) == true) {

			return true;
		}
	}

	return false;
}

bool SceneNode::removeChildNode(string childNodeName, int depth) {

	if(depth == 0)
		return false;

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++) {

		GraphicObject* graphicObject = sceneNodeIterator->second->getGraphicObject();

		if(graphicObject->getName().compare(childNodeName) == 0) {

			graphicObject->setParentName("None");

			_sceneNodeMap.erase(childNodeName);

			return true;
		}

		if(sceneNodeIterator->second->removeChildNode(childNodeName,depth - 1) == true) {

			return true;
		}
	}

	return false;
}

SceneNode* SceneNode::getChildNode(string childNodeName) {

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++) {

		GraphicObject* graphicObject = sceneNodeIterator->second->getGraphicObject();

		if(graphicObject->getName().compare(childNodeName) == 0)
			return sceneNodeIterator->second;

		SceneNode* sceneNode = sceneNodeIterator->second->getChildNode(childNodeName);

		if(sceneNode != NULL)
			return sceneNode;
	}

	return NULL;
}

SceneNode* SceneNode::getChildNode(string childNodeName, int depth) {

	if(depth == 0)
		return NULL;

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++) {

		GraphicObject* graphicObject = sceneNodeIterator->second->getGraphicObject();

		if(graphicObject->getName().compare(childNodeName) == 0)
			return sceneNodeIterator->second;

		SceneNode* sceneNode = sceneNodeIterator->second->getChildNode(childNodeName,depth - 1);

		if(sceneNode != NULL)
			return sceneNode;
	}

	return NULL;
}

void SceneNode::setGraphicObject(GraphicObject* graphicObject) {

	_graphicObject = graphicObject;
}

GraphicObject* SceneNode::getGraphicObject() {

	return _graphicObject;
}	

void SceneNode::setShaderProgram(ShaderProgram* shaderProgram) {

	_shaderProgram = shaderProgram;
}

ShaderProgram* SceneNode::getShaderProgram() {

	return _shaderProgram;
}

void SceneNode::setName(const char* name) {

	_name.assign(name);
}

string SceneNode::getName() {

	return _name;
}

void SceneNode::dump() {

	cout << "<SceneNode Dump> = " << _name << endl;

	/* Scene Node Map */
	cout << "<SceneNode Node List> = " << endl;
	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->dump();
}