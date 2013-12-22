#ifndef JOINT_OBJECT_H
#define JOINT_OBJECT_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "Object.h"

using namespace std;

class JointObject : public Object {

	protected:

		Vector _jointPosition;

	public:

		/* Constructors & Destructors */
		JointObject(string name);
		~JointObject();

		/* Scene Methods */
		void draw(GLuint programID);

		void update();
		void update(GLfloat elapsedTime);

		/* Getters */
		Vector getJointPosition();

		/* Setters */
		void setJointPosition(Vector jointPosition);

		/* Debug Methods */
		void dump();
};

#endif