#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include "Utility.h"

class FrameBuffer {

	private:

		/* Singleton Instance */
		static FrameBuffer* instance;

		/* Buffer Dimensions */
		GLint _width;
		GLint _height;
		
		/* Buffer Handlers */
		GLuint _frameBufferObject;
		GLuint _frameBufferTexture[2];

		GLuint _renderBufferObject;

		FrameBuffer();
		~FrameBuffer();

	public:

		/* Singleton Methods */
		static FrameBuffer* getInstance();
		static void destroyInstance();

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