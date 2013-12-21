#ifndef TABLE_WALL_H
#define TABLE_WALL_H

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

class TableWall : public GraphicObject {

	protected:

	public:

		/* Constructors & Destructors */
		TableWall(const char* name);
		~TableWall();

		/* Scene Methods */
		void draw(GLuint programID, GLuint uniformID);

		void update(GLfloat elapsedTime);
		void updateCollision();

		/* Getters */
		/* Setters */

		/* Debug Methods */
		void dump();
};

#endif