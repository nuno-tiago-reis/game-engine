#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "BoundingVolume.h"

#define BOUNDING_SPHERE "Bounding Sphere"

using namespace std;

class GraphicObject;

class BoundingSphere: public BoundingVolume {

	protected:

		/* Bounding Sphere Radius */
		GLfloat _radius;

	public:

		/* Constructors & Destructors */
		BoundingSphere(GLfloat radius);

		~BoundingSphere();

		/* Update Methods */
		void update(GraphicObject* graphicObject);

		void update(GraphicObject* graphicObject, GLfloat elapsedTime);

		/* Getters & Setters */
		GLfloat getRadius();

		void setRadius(GLfloat radius);

		/* Debug Methods */
		void dump();
};

#endif