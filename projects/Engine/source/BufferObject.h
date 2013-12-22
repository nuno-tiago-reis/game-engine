#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include "ShaderProgram.h"

#include "Utility.h"

typedef struct {

	GLfloat position[4];
	
	GLfloat normal[4];
	GLfloat tangent[4];
	GLfloat textureUV[2];

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat specularConstant;

} Vertex;

using namespace std;

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