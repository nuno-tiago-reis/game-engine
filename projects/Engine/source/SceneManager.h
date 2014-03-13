#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

/* FMODEX Sound Library */
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#define MAX_SOUND_CHANNELS 32

/* FreeGlut Library */
#include "GL/glew.h"
#include "GL/glut.h"

/* STL Data Storage and Iteration */
#include <map>
#include <iterator>

/* Matrix - Ray Cast */
#include "Matrix.h"

/* Light - Light Map */
#include "Light.h"
/* Camera - Camera Map */
#include "Camera.h"
/* Shader - Shader Map */
#include "ShaderProgram.h"
/* Sound - Sound Map */
#include "Sound.h"

/* SceneNode - SceneNode Map*/
#include "SceneNode.h"

/* Object - Object Map*/
#include "Object.h"
#include "JointObject.h"

/* Noise Generator */
#include "PerlinNoise.h"

/* User Interaction Handlers */
#include "KeyboardHandler.h"
#include "MouseHandler.h"

/* Error Checking */
#include "Utility.h"

#include "Names.h"

class SceneManager {

	private:

		/* Singleton Instance */
		static SceneManager *instance;

		int _rotationAxis;
		int _currentObject;

		/* Picking */
		bool _malletPicked;
		GLfloat _malletDepth;

		/* Interpolation */
		GLfloat _timeCounter;

		/* FMOD Sound System */
		FMOD::System* _fmodSystem;
		FMOD::Channel* channel[MAX_SOUND_CHANNELS];

		/* Box2D Physics World */
		
		/* Active Camera */
		Camera* _activeCamera;
		/* Active Shader Program */
		ShaderProgram* _activeShaderProgram;

		/* Sound Map */
		map<string,Sound*> _soundMap;
		/* Light Map  */
		map<string,Light*> _lightMap;
		/* Camera Map  */
		map<string,Camera*> _cameraMap;
		/* Shader Program Map  */
		map<string,ShaderProgram*> _shaderProgramMap;
		/* Object Map  */
		map<string,Object*> _objectMap;
		/* Scene Graph Map  */
		map<string,SceneNode*> _sceneNodeMap;

		/* Constructors & Destructors */
		SceneManager();
		~SceneManager();

	public:

		/* Singleton Methods */
		static SceneManager* getInstance();
		static void destroyInstance();

		/* Scene Methods */
		void init();

		void loadUniforms();

		void draw();

		void update(GLfloat elapsedTime);

		void reshape(GLint width, GLint height);

		void rotateJoint(Object* object, Vector rotationDelta, GLfloat elapsedTime);

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

		/* Sound Map Manipulation Methods */
		void addSound(Sound* sound);
		void removeSound(string soundName);

		Sound* getSound(string soundName);

		/* Light Map Manipulation Methods */
		void addLight(Light* light);
		void removeLight(string lightName);

		Light* getLight(string lightName);

		/* Camera Map Manipulation Methods */
		void addCamera(Camera* camera);
		void removeCamera(string cameraName);

		Camera* getCamera(string cameraName);

		/* Shader Map Manipulation Methods */
		void addShaderProgram(ShaderProgram* shaderProgram);
		void removeShaderProgram(string shaderProgramName);

		ShaderProgram* getShaderProgram(string shaderProgramName);

		/* Object Map Manipulation Methods */
		void addObject(Object* graphicObject);
		void removeObject(string graphicObjectName);

		Object* getObject(string graphicObjectName);

		/* Scene Node Map Manipulation Methods */
		void addSceneNode(SceneNode* sceneNode);
		void removeSceneNode(string sceneNodeName);

		SceneNode* getSceneNode(string sceneNodeName);

		/* Debug Methods */
		void dump();
};

#endif