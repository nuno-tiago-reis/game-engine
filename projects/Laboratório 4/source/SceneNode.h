#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include "ShaderProgram.h"
#include "GraphicObject.h"

class SceneNode {

	private:

		/* Scene Node Identifier */
		string _name;

		/* Scene Node Shader */
		ShaderProgram* _shaderProgram;

		/* Scene Node Graphic Object */
		GraphicObject* _graphicObject;

		/* Scene Node Map */
		map<string,SceneNode*> _sceneNodeMap;

	public:
		
		/* Constructors & Destructors */
		SceneNode(const char* name);
		~SceneNode();

		/* Scene Graph Methods */
		void draw(ShaderProgram* shader);

		void update();
		void update(GLfloat elapsedTime);

		void updateBoundingVolume(GLfloat elapsedTime);

		/* GraphicObject List Manipulation Methods */
		void addChildNode(SceneNode* sceneNode);

		bool removeChildNode(string childNodeName);

		bool removeChildNode(string childNodeName, int depth);

		SceneNode* getChildNode(string childNodeName);

		SceneNode* getChildNode(string childNodeName, int depth);

		/* Graphic Object Manipulation Methods */
		void setGraphicObject(GraphicObject* graphicObject);

		GraphicObject* getGraphicObject();	

		/* Shader Manipulation Methods */
		void setShaderProgram(ShaderProgram* shaderProgram);

		ShaderProgram* getShaderProgram();	

		/* Getters */
		void setName(const char* name);

		string getName();

		/* Debug Methods */
		void dump();
};

#endif