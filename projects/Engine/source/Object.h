#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"

#include <map>
#include <string>

#include "Matrix.h"

#include "BufferObject.h"

#include "RealWoodShader.h"
#include "ShaderProgram.h"

#include "Texture.h"
#include "GeneratedTexture.h"

#define NO_MAPPING 0
#define WOOD_MAPPING 1
#define BUMP_MAPPING 2

#define WOOD_TEXTURE "Wood Texture"
#define DIFFUSE_TEXTURE "Diffuse Texture"
#define NORMAL_TEXTURE "Normal Texture"

using namespace std;

class Object {

	protected:

		/* Graphic Object Identifier */
		string _name;
		string _parentName;

		/* Graphic Object Model Matrix */
		Matrix _modelMatrix;

		/* Graphic Object BufferObject */
		BufferObject* _bufferObject;

		/* Graphic Object Position Vector (Px,Py,Pz,1.0) */
		Vector _position;
		/* Graphic Object Velocity Vector*/
		Vector _velocity;
		/* Graphic Object Scale Factors (Sx,Sy,Sz,1.0) */
		Vector _scale;
		/* Graphic Object Rotation (Rx,Ry,Rz,1.0) */
		Vector _rotation;
		/* Graphic Object Rotation Quaternion */
		Quaternion _rotationQuaternion;

		/* Graphic Object Active Texture */
		GLuint _activeTexture;
		/* Graphic Object Texture Map */
		map<string,Texture*> _textureMap;

	public:

		/* Constructors & Destructors */
		Object(string name);
		~Object();

		/* Scene Methods */
		virtual void draw(GLuint programID);

		virtual void update();
		virtual void update(GLfloat elapsedTime);

		/* Ray Cast Intersection Method */
		GLfloat isIntersecting(Vector origin, Vector direction);

		/* Texture Methods*/
		void activateBumpTexture(string diffuseTextureFileName, string normalTextureFileName);
		void bindBumpTexture(GLuint programID);
		void unbindBumpTexture();

		void activateWoodTexture(GLfloat noiseAlpha, GLfloat noiseBeta, GLint noiseOctaves, GLfloat noiseScale);
		void bindWoodTexture(GLuint programID);
		void unbindWoodTexture();

		void deactivateTexture();

		/* Getters */
		string getName();
		string getParentName();

		Matrix getModelMatrix();

		BufferObject* getBufferObject();

		Vector getPosition();
		Vector getVelocity();
		Vector getScale();
		Vector getRotation();
		Quaternion getRotationQuaternion();

		/* Setters */
		void setName(string name);
		void setParentName(string parentName);

		void setModelMatrix(Matrix modelMatrix);

		void setBufferObject(BufferObject* bufferObject);

		void setPosition(Vector position);
		void setVelocity(Vector velocity);
		void setScale(Vector scale);
		void setRotation(Vector rotation);
		void setRotationQuaternion(Quaternion rotationQuaternion);

		/* Debug Methods */
		void dump();
};

#endif