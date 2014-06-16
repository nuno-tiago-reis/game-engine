#ifndef POST_PROCESSING_EFFECT_H
#define POST_PROCESSING_EFFECT_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include "FrameBuffer.h"

#include "Utility.h"
#include "Constants.h"

/* Position Attribute */
#define POSITION 0
#define POSITION_ATTRIBUTE "VertexPosition"

class PostProcessingEffect {

	protected:

		/* Post Processing Effect Identifier */
		string _name;

		/* Post Processing Effect Frame Buffer */
		FrameBuffer* _frameBuffer;

		/* Vertex Array IDs */
		GLuint _vertexArrayObjectID;
		GLuint _vertexBufferObjectID;

	public:

		/* Constructors & Destructors */
		PostProcessingEffect(string name);
		~PostProcessingEffect();

		/* Scene Methods */
		void init(GLint width, GLint height);

		virtual void draw() = 0;

		virtual void reshape(GLint width, GLint height) = 0;

		/* Getters */
		string getName();

		FrameBuffer* getFrameBuffer();

		GLuint getVertexArrayObjectID();
		GLuint getVertexBufferObjectID();

		/* Setters */
		void setName(string name);

		void setFrameBuffer(FrameBuffer* frameBuffer);

		void setVertexArrayObjectID(GLuint vertexArrayObjectID);
		void setVertexBufferObjectID(GLuint vextexBufferObjectID);
};

#endif