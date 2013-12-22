#include "SceneNode.h"

SceneNode::SceneNode(string name) {

	_name.assign(name);

	_shaderProgram = NULL;

	_graphicObject = NULL;
}

SceneNode::~SceneNode() {

	delete _graphicObject;

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		delete sceneNodeIterator->second;
}

void SceneNode::draw(ShaderProgram* shaderProgram) {

	if(shaderProgram != NULL && _shaderProgram == NULL)
		_shaderProgram = shaderProgram;

	if(_shaderProgram == NULL) {

		cerr << "<Scene Node> Shader not initialized." << endl;
		return;
	}

	if(_name.compare("Platform") != 0)
		_graphicObject->draw(_shaderProgram->getProgramID());

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->draw(_shaderProgram);
}

void SceneNode::update() {

	MatrixStack* matrixStack = MatrixStack::getInstance();

	/* Update Node Matrix with Matrix Stack */
	_graphicObject->setModelMatrix(matrixStack->peek());
	_graphicObject->update();

	/* Push Model Matrix to the Matrix Stack */
	matrixStack->push(_graphicObject->getModelMatrix());

	/* Update Child Nodes */
	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->update();

	/* Pop Model Matrix to the Matrix Stack */
	matrixStack->pop();

	/* Update Scale outside Matrix Stack */
	Vector scale = _graphicObject->getScale();

	Matrix modelMatrix = _graphicObject->getModelMatrix();
	modelMatrix.scale(scale[0],scale[1],scale[2]);

	_graphicObject->setModelMatrix(modelMatrix);
}

void SceneNode::update(GLfloat elapsedTime) {

MatrixStack* matrixStack = MatrixStack::getInstance();

	/* Update Node Matrix with Matrix Stack */
	_graphicObject->setModelMatrix(matrixStack->peek());
	_graphicObject->update(elapsedTime);

	/* Push Model Matrix to the Matrix Stack */
	matrixStack->push(_graphicObject->getModelMatrix());

	/* Update Child Nodes */
	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++)
		sceneNodeIterator->second->update(elapsedTime);

	/* Pop Model Matrix to the Matrix Stack */
	matrixStack->pop();

	/* Update Scale outside Matrix Stack */
	Vector scale = _graphicObject->getScale();

	Matrix modelMatrix = _graphicObject->getModelMatrix();
	modelMatrix.scale(scale[0],scale[1],scale[2]);

	_graphicObject->setModelMatrix(modelMatrix);
}

void SceneNode::addChildNode(SceneNode* sceneNode) {

	sceneNode->getObject()->setParentName(_graphicObject->getName().c_str());

	_sceneNodeMap[sceneNode->getObject()->getName()] = sceneNode;
}

bool SceneNode::removeChildNode(string childNodeName) {

	for(map<string,SceneNode*>::const_iterator sceneNodeIterator = _sceneNodeMap.begin(); sceneNodeIterator != _sceneNodeMap.end(); sceneNodeIterator++) {

		Object* graphicObject = sceneNodeIterator->second->getObject();

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

		Object* graphicObject = sceneNodeIterator->second->getObject();

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

		Object* graphicObject = sceneNodeIterator->second->getObject();

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

		Object* graphicObject = sceneNodeIterator->second->getObject();

		if(graphicObject->getName().compare(childNodeName) == 0)
			return sceneNodeIterator->second;

		SceneNode* sceneNode = sceneNodeIterator->second->getChildNode(childNodeName,depth - 1);

		if(sceneNode != NULL)
			return sceneNode;
	}

	return NULL;
}

void SceneNode::setObject(Object* graphicObject) {

	_graphicObject = graphicObject;
}

Object* SceneNode::getObject() {

	return _graphicObject;
}	

void SceneNode::setShaderProgram(ShaderProgram* shaderProgram) {

	_shaderProgram = shaderProgram;
}

ShaderProgram* SceneNode::getShaderProgram() {

	return _shaderProgram;
}

void SceneNode::setName(string name) {

	_name = name;
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