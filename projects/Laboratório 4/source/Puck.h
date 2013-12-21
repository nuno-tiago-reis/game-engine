#ifndef PUCK_H
#define PUCK_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "GraphicObject.h"

#include "Constants.h"

using namespace std;

class Puck : public GraphicObject {

	protected:

	public:

		/* Constructors & Destructors */
		Puck(const char* name);
		~Puck();

		/* Scene Methods */

		void update(GLfloat elapsedTime);
		void updateCollision();

		/* Getters */
		/* Setters */

		/* Debug Methods */
		void dump();
};

#endif