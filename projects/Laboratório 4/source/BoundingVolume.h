#ifndef BOUNDING_VOLUME_H
#define BOUNDING_VOLUME_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "GraphicObject.h"

#include "Vector.h"
#include "Constants.h"

#define BOUNDING_VOLUME "Bounding Volume"

using namespace std;

class GraphicObject;

class BoundingVolume {

	protected:

		/* Bounding Volume Type */
		string _type;
		string _collidingVolumeType;

		/* Bounding Volume Position */
		Vector _position;
		Vector _impactPosition;

		Vector _velocity;
		Vector _impactVelocity;

		/* Bounding Volume Collision Axis */
		Vector _collisionPlaneX;
		Vector _collisionPlaneY;
		Vector _collisionPlaneN;

		GLfloat _elapsedTime;

	public:

		/* Constructors & Destructors */
		BoundingVolume();

		~BoundingVolume();

		/* Update Methods */
		virtual void update(GraphicObject* graphicObject);

		virtual void update(GraphicObject* graphicObject, GLfloat elapsedTime);

		/* Getters & Setters */
		string getType();
		string getCollidingVolumeType();

		bool isColliding();
		
		Vector getPosition();
		Vector getImpactPosition();

		Vector getVelocity();
		Vector getImpactVelocity();

		Vector getCollisionPlaneX();
		Vector getCollisionPlaneY();
		Vector getCollisionPlaneN();

		GLfloat getElapsedTime();

		void setType(const char* name);
		void setCollidingVolumeType(const char* name);

		void setCollision(bool collision);

		void setPosition(Vector position);
		void setImpactPosition(Vector impactPosition);

		void setVelocity(Vector velocity);
		void setImpactVelocity(Vector impactVelocity);

		void setCollisionPlaneX(Vector collisionPlaneX);
		void setCollisionPlaneY(Vector collisionPlaneY);
		void setCollisionPlaneN(Vector collisionPlaneN);

		void setElapsedTime(GLfloat elapsedTime);

		/* Debug Methods */
		virtual void dump();
};

#endif