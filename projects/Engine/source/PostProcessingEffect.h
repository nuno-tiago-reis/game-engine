#ifndef POST_PROCESSING_EFFECT_H
#define POST_PROCESSING_EFFECT_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

/* OpenGL definitions */
#include "GL/glew.h"
#include "GL/glut.h"
/* OpenGL Error Check */
#include "Utility.h"

/* C++ Includes */
#include <string>

/* Post-processing Frame Buffer */
#include "FrameBuffer.h"

/* Engine Constants */
#include "Constants.h"

/* Position Attribute */
#define POSITION 0
#define POSITION_ATTRIBUTE "VertexPosition"

class PostProcessingEffect {

	protected:

		/* Post Processing Effect Identifier */
		string name;

		/* Post Processing Effect Frame Buffer */
		FrameBuffer* frameBuffer;

		/* Vertex Array IDs */
		GLuint arrayObjectID;
		GLuint bufferObjectID;

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

		GLuint getArrayObjectID();
		GLuint getBufferObjectID();

		/* Setters */
		void setName(string name);

		void setFrameBuffer(FrameBuffer* frameBuffer);

		void setArrayObjectID(GLuint arrayObjectID);
		void setBufferObjectID(GLuint bufferObjectID);
};

#endif