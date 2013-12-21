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
#include <array>

#include "Matrix.h"
#include "MatrixStack.h"

#include "BufferObject.h"

#include "BoundingVolume.h"

#include "Constants.h"

#include "LightingManager.h"

#include "Texture.h"
#include "GeneratedTexture.h"

using namespace std;

class BoundingVolume;

class GraphicObject {

	protected:

		/* Graphic Object Identifier */
		string _name;
		string _parentName;

		/* Graphic Object Model Matrix */
		Matrix _modelMatrix;

		/* Graphic Object BufferObject */
		BufferObject* _bufferObject;

		/* Graphic Object Collision Attributes */
		GLfloat _bounce;
		GLfloat _friction;

		BoundingVolume* _boundingVolume;

		/* Graphic Object Current and Final Scale Factor*/
		Vector _scale;

		/* Graphic Object Current and Final Rotation & Corresponding Quaternions*/
		Vector _rotation;
		
		Quaternion _rotationQuaternion;

		/* Graphic Object Current and Final Position & Velocity Vector*/
		Vector _position;

		Vector _velocity;

		bool bumpActive;
		Texture* diffuseTexture;
		Texture* normalTexture;

		bool woodActive;
		Texture* noiseTexture;
		GLfloat noiseScale;
		

	public:

		/* Constructors & Destructors */
		GraphicObject(const char* name);
		~GraphicObject();

		/* Scene Methods */
		virtual void draw(GLuint programID, GLuint uniformID);

		virtual void update();
		virtual void update(GLfloat elapsedTime);

		virtual void updateCollision();
		virtual void updateBoundingVolume(GLfloat elapsedTime);

		GLfloat isIntersecting(Vector origin, Vector direction);
		void activateBump( const char* diffuse_path, const char* normal_path);
		void activateWood(GLfloat alpha, GLfloat beta, GLfloat octaves, GLfloat Scale);

		/* Getters */
		string getName();
		string getParentName();

		Matrix getModelMatrix();

		BufferObject* getBufferObject();

		GLfloat getBounce();
		GLfloat getFriction();
		BoundingVolume* getBoundingVolume();

		Vector getScale();

		Vector getRotation();

		Quaternion getRotationQuaternion();

		Vector getPosition();

		Vector getVelocity();

		/* Setters */
		void setName(const char* name);
		void setParentName(const char* parentName);

		void setModelMatrix(Matrix modelMatrix);

		void setBufferObject(BufferObject* bufferObject);

		void setBounce(GLfloat bounce);
		void setFriction(GLfloat friction);
		void setBoundingVolume(BoundingVolume* boundingVolume);

		void setScale(Vector scale);

		void setRotation(Vector rotation);

		void setPosition(Vector position);

		void setVelocity(Vector velocity);

		/* Debug Methods */
		void dump();
};

#endif