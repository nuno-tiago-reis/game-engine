#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

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

class FrameBuffer {

	private:

		/* Buffer Dimensions */
		GLint width;
		GLint height;
		
		/* Buffer Handlers */
		GLuint frameBufferObject;
		GLuint frameBufferTexture[2];

		GLuint renderBufferObject;

	public:

		/* Constructors & Destructors */
		FrameBuffer();
		~FrameBuffer();

		/* Scene Methods */
		void init(GLint width, GLint height);

		void reshape(GLint width, GLint height);

		/* Getters */
		GLint getWidth();
		GLint getHeight();

		GLuint getFrameBufferObject();
		GLuint getFrameBufferTexture(int index);

		GLuint getRenderBufferObject();

		/* Setters */
		void setWidth(GLint width);
		void setHeight(GLint height);

		void setFrameBufferObject(GLuint frameBufferObjectHandler);
		void setFrameBufferTexture(GLuint frameBufferTextureHandler, int index);

		void setRenderBufferObject(GLuint renderBufferObject);
};

#endif