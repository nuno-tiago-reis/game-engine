#ifndef MATRIX_H
#define MATRIX_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include <math.h>

#include "Vector.h"
#include "Quaternion.h"

#include "Constants.h"

#define M_0_0 0
#define M_0_1 1
#define M_0_2 2
#define M_0_3 3

#define M_1_0 4
#define M_1_1 5
#define M_1_2 6
#define M_1_3 7

#define M_2_0 8
#define M_2_1 9
#define M_2_2 10
#define M_2_3 11

#define M_3_0 12
#define M_3_1 13
#define M_3_2 14
#define M_3_3 15

/* Translation Matrix Constants */
#define T_X 3
#define T_Y 7
#define T_Z 11

/* Scale Matrix Constants */
#define S_X 0
#define S_Y 5
#define S_Z 10

/* Rotation Matrix Constants */
#define R1_X 5
#define R2_X 6
#define R3_X 9
#define R4_X 10

#define R1_Y 0
#define R2_Y 2
#define R3_Y 8
#define R4_Y 10

#define R1_Z 0
#define R2_Z 1
#define R3_Z 4
#define R4_Z 5

using namespace std;

class Matrix {

	private:

		/* Matrix Content */
		GLfloat _matrix[4][4];

	public:

		static const GLfloat threshold;
		
		/* Constructors & Destructors */
		Matrix();
		Matrix(Quaternion quaternion);
		Matrix(GLfloat initialValue);
		Matrix(const GLfloat initialValue[16]);
		Matrix(const GLfloat initialValue[4][4]);

		/* View Matrix Constructor */
		Matrix(const GLfloat xAxis[3], const GLfloat yAxis[3], const GLfloat zAxis[3]);	

		~Matrix();

		/* Identity Transformation */
		void loadIdentity();
		void clean();

		/* Graphical Transformations */
		void scale(GLfloat xScaling, GLfloat yScaling, GLfloat zScaling);
		void rotate(GLfloat angle, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);
		void translate(GLfloat xCoordinate, GLfloat yCoordinate, GLfloat zCoordinate);
		void quaternionRotate(Quaternion quaternion);
		void transpose();
		void invert();

		/* Camera Transformations */
		void setView(Vector eye, Vector center, Vector userUp);
		void setOrthogonalProjection(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat nearZ, GLfloat farZ);
		void setPerspectiveProjection(GLfloat fieldOfView, GLfloat aspectRatio, GLfloat nearZ, GLfloat farZ);

		/* Getters */
		GLfloat* getValue();
		GLfloat getValue(int row, int column);

		/* Setters */
		void setValue(const GLfloat value[16]);
		void setValue(int row, int column, GLfloat value);

		/* Operators */
		GLfloat& operator[] (int position);

		Matrix operator +  (Matrix matrix);
		Matrix operator += (Matrix matrix);
		Matrix operator *  (Matrix matrix);
		Matrix operator *= (Matrix matrix);

		Vector operator *  (Vector vector);

		bool operator == (Matrix matrix);

		/* Debug */
		void dump();
};

#endif