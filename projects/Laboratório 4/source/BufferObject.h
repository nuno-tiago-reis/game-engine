#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include "Matrix.h"

#include "Utility.h"
#include "Constants.h"

using namespace std;

typedef struct {

	GLfloat XYZW[4];
	GLfloat RGBA[4];
	GLfloat NORMAL[4];
	GLfloat TEXUV[2];
	GLfloat AMBIENT[4];
	GLfloat TANGENT[4];
	GLfloat SPECULAR[4];
	GLfloat SPECULAR_CONSTANT[1];

} Vertex;

class BufferObject {

	protected:

		/* Buffer Object Vertex Attributes */
		GLint _vertexCount;
		Vertex* _vertices;

		/* Buffer Object OpenGL IDs */
		GLuint _vertexArrayObjectID;
		GLuint _vertexBufferObjectID;

		bool woodenObject;
		GLfloat noiseScale;

	public:
		
		/* Constructors & Destructors */
		BufferObject(const Vertex* vertices, GLint vertexCount);
		~BufferObject();

		/* GPU Creation & Destruction Methods */
		void createBufferObject();

		void destroyBufferObject();

		/* Getters */
		GLint getVertexCount();
		Vertex* getVertices();

		GLuint getVertexArrayObjectID();
		GLuint getVertexBufferObjectID();

		/* Setters */
		void setVertexCount(GLint vertexCount);
		void setVertices(Vertex* vertices, GLint vertexCount);

		void setVertexArrayObjectID(GLuint vertexArrayObjectID);
		void setVertexBufferObjectID(GLuint vextexBufferObjectID);

		/* Debug Methods */
		void dump();
};

#endif