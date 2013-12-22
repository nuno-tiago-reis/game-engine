#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "BoundingVolume.h"

#define BOUNDING_BOX "Bounding Box"

using namespace std;

class GraphicObject;

class BoundingBox: public BoundingVolume {

	protected:

		/* Bounding Box Dimensions */
		GLfloat _height;
		GLfloat _width;
		GLfloat _depth;

		/* Bounding Box Orientation */
		Vector _xAxis;
		Vector _yAxis;
		Vector _zAxis;
		
		/* Bounding Box Corners */
		map<int,Vector> _cornerMap;

	public:

		/* Constructors & Destructors */
		BoundingBox(GLfloat side);

		BoundingBox(GLfloat height, GLfloat width);

		BoundingBox(GLfloat height, GLfloat width, GLfloat depth);

		~BoundingBox();

		/* Update Methods */
		void update(GraphicObject* graphicObject);

		void update(GraphicObject* graphicObject, GLfloat elapsedTime);

		/* Getters & Setters */
		GLfloat getHeight();
		GLfloat getWidth();
		GLfloat getDepth();

		Vector getXAxis();
		Vector getYAxis();
		Vector getZAxis();

		Vector getCorner(int cornerID);

		void setHeight(GLfloat height);
		void setWidth(GLfloat width);
		void setDepth(GLfloat depth);

		void setXAxis(Vector xAxis);
		void setYAxis(Vector yAxis);
		void setZAxis(Vector zAxis);

		void addCorner(Vector corner, int cornerID);
		void removeCorner(int corner);

		/* Debug Methods */
		void dump();
};

#endif