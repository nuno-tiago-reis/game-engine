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

/* Object Names */
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

/* Joint Object Names */
#define DRAGON_HEAD "Dragon Head"
#define DRAGON_NOSE "Dragon Nose"

#define DRAGON_LEFT_HORN "Dragon Left Horn"
#define DRAGON_RIGHT_HORN "Dragon Right Horn"

#define DRAGON_LEFT_WING "Dragon Left Wing"
#define DRAGON_LEFT_WING_2 "Dragon Left Wing 2"

#define DRAGON_RIGHT_WING "Dragon Right Wing"
#define DRAGON_RIGHT_WING_2 "Dragon Right Wing 2"

#define DRAGON_BODY "Dragon Body"

#define DRAGON_TAIL "Dragon Tail"
#define DRAGON_TAIL_2 "Dragon Tail 2"
#define DRAGON_TAIL_3 "Dragon Tail 3"

#define ARROW_SOUND_NAME "Arrow Sound"
#define ARROW_SOUND_FILE "sounds/arrow.mp3"

#define CANNON_SOUND_NAME "Cannon Sound"
#define CANNON_SOUND_FILE "sounds/cannon.mp3"

#define PUNCH_SOUND_NAME "Punch Sound"
#define PUNCH_SOUND_FILE "sounds/punch.mp3"

#define SWEEPER_SOUND_NAME "Sweeper Sound"
#define SWEEPER_SOUND_FILE "sounds/sweeper.mp3"

#define MUSIC_SOUND_NAME "Underwear"
#define MUSIC_SOUND_FILE "sounds/Royal Republic - I can see your Underwear.mp3"

#define RAYQUAZA_SOUND_NAME "Rayquaza"
#define RAYQUAZA_SOUND_FILE "sounds/rayquaza.ogg"

class SceneManager {

	private:

		/* Singleton Instance */
		static SceneManager *instance;

		/* Picking */
		bool _malletPicked;
		GLfloat _malletDepth;

		/* Interpolation */
		GLfloat _timeCounter;

		/* FMOD Sound System */
		FMOD::System* _fmodSystem;
		FMOD::Channel* channel[MAX_SOUND_CHANNELS];
		
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