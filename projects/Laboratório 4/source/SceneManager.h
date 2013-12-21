#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include <utility>

#include <iterator>
#include <string>
#include <map>

#include "KeyboardHandler.h"
#include "MouseHandler.h"

#include "Matrix.h"
#include "Camera.h"
#include "ShaderProgram.h"

#include "SceneNode.h"
#include "GraphicObject.h"

#include "CollisionTest.h"

#include "LightingManager.h"

#include "Utility.h"

#define SCENE_GRAPH_TESTING	false

#define PUCK "Puck"
#define MALLET "Mallet"

#define TABLE "Table"
#define TABLE_SURFACE "Table Surface"

#define WALL_A "Wall A"
#define WALL_B "Wall B"
#define WALL_C "Wall C"
#define WALL_D "Wall D"

#define PLATFORM "Platform"

#define PIN_A "Pin A"
#define PIN_B "Pin B"
#define PIN_C "Pin C"

#define DRAGON_HEAD "Dragon Head"
#define DRAGON_LEFT_ARM "Dragon Left Arm"
#define DRAGON_RIGHT_ARM "Dragon Right Arm"
#define DRAGON_BODY "Dragon Body"
#define DRAGON_TAIL "Dragon Tail"
#define DRAGON_TAIL_2 "Dragon Tail 2"

class SceneManager {

	private:

		/* Singleton Instance */
		static SceneManager *instance;

		/* Picking */
		bool _malletPicked;
		GLfloat _malletDepth;
		GLfloat _timeCounter;
		
		/* Active Camera */
		Camera* _activeCamera;
		/* Active Shader Program */
		ShaderProgram* _activeShaderProgram;

		/* Camera Map  */
		map<string,Camera*> _cameraMap;
		/* Shader Program Map  */
		map<string,ShaderProgram*> _shaderProgramMap;

		/* Scene Graph Map  */
		map<string,SceneNode*> _sceneNodeMap;
		/* GraphicObject Map  */
		map<string,GraphicObject*> _graphicObjectMap;

		/* Constructors & Destructors */
		SceneManager();
		~SceneManager();

	public:

		/* Singleton Methods */
		static SceneManager* getInstance();
		static void destroyInstance();

		/* Scene Methods */
		void loadUniforms();

		void draw();

		void update(GLfloat elapsedTime);

		void reshape(GLint width, GLint height);

		/* Collision Methods */
		void testCollision(GraphicObject* objectA, GraphicObject* objectB);

		/* Scene Interaction Methods */
		void readKeyboard(GLfloat elapsedTime);
		void readMouse(GLfloat elapsedTime);

		void rayCast(GLint* mousePosition, GLfloat elapsedTime);

		/* Getters */
		Camera* getActiveCamera();
		ShaderProgram* getActiveShaderProgram();

		/* Setters */
		void setActiveCamera(Camera* camera);
		void setActiveShaderProgram(ShaderProgram* shaderProgram);

		/* Camera Map Manipulation Methods */
		void addCamera(Camera* camera);
		void removeCamera(string cameraName);

		Camera* getCamera(string cameraName);

		/* Shader Map Manipulation Methods */
		void addShaderProgram(ShaderProgram* shaderProgram);
		void removeShaderProgram(string shaderProgramName);

		ShaderProgram* getShaderProgram(string shaderProgramName);

		/* GraphicObject Map Manipulation Methods */
		void addGraphicObject(GraphicObject* graphicObject);
		void removeGraphicObject(string graphicObjectName);

		GraphicObject* getGraphicObject(string graphicObjectName);

		/* Scene Node Map Manipulation Methods */
		void addSceneNode(SceneNode* sceneNode);
		void removeSceneNode(string sceneNodeName);

		SceneNode* getSceneNode(string sceneNodeName);

		/* Debug Methods */
		void dump();
};

#endif