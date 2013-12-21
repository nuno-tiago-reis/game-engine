#ifndef MALLET_H
#define MALLET_H

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

class Mallet : public GraphicObject {

	protected:

	public:

		/* Constructors & Destructors */
		Mallet(const char* name);
		~Mallet();

		/* Scene Methods */
		void update(GLfloat elapsedTime);
		void updateCollision();

		/* Getters */
		/* Setters */

		/* Debug Methods */
		void dump();
};

#endif