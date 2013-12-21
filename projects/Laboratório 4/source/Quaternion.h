#ifndef QUATERNION_H
#define QUATERNION_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include <iostream>
#include <string>
#include <cassert>
#include <utility>
#include <math.h>

#include "Vector.h"
#include "Constants.h"

using namespace std;

#define QT 0
#define QX 1
#define QY 2
#define QZ 3

#define HEADER(X) std::cout<<std::endl<<(X)<<std::endl<<std::endl;

class Quaternion {

	private:

		static const GLfloat threshold;

		/* Quaternion Content */
		GLfloat _quaternion[4];

	public:
		
		/* Constructors & Destructors */
		Quaternion();
		Quaternion(GLfloat* initialValue);
		Quaternion(GLfloat theta, Vector axis);			
		~Quaternion();

		/* Quaternion Conversions */
		void toAngle(GLfloat* theta, Vector* axis);		

		/* Quaternion Transformations */
		void clean();									
		void invert();								
		void conjugate();								
		void normalize();								
		GLfloat norm();								
		GLfloat quadrance();							

		/* Quaternion Operations */
		Quaternion operator+ (Quaternion quaternion);
		Quaternion operator+= (Quaternion quaternion);
		Quaternion operator* (GLfloat scalar);
		Quaternion operator*= (GLfloat scalar);
		Quaternion operator* (Quaternion quaternion);
		Quaternion operator*= (Quaternion quaternion);

		void lerp(Quaternion quaternion, GLfloat k);	
		void slerp(Quaternion quaternion, GLfloat k);	
		bool operator== (Quaternion quaternion);		
		GLfloat& operator[] (int position);
		GLfloat* getValue();
		void setValue(GLfloat* value);

		/* Debug */
		void dump();							
};

#endif