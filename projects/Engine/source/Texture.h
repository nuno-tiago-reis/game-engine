#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "soil.h"

#include <string>

#include "Utility.h"

class Texture {

	protected:

		/* Texture FileName */
		string _fileName;

		/* OpenGL Texture Handler */
		GLuint _textureHandler;

		/* OpenGL Texture Format (ex. GL_TEXTURE_2D) */
		GLenum _textureFormat;

	public:

		/* Constructors & Destructors */
		Texture(GLuint textureFormat, string fileName);
		Texture(GLuint textureFormat);
		Texture();

		~Texture();

		/* Loading Methods */
		virtual void load();

		/* Bind & Unbind to OpenGL Methods */
		virtual void bind(GLuint textureID);
		virtual void unbind(GLuint textureID);

		/* Getters */
		string getFileName();

		GLuint getTextureHandler();

		GLenum getTextureFormat();

		/* Setters */
		void setFileName(string fileName);

		void setTextureHandler(GLuint textureHandler);

		void setTextureFormat(GLenum textureFormat);
};

#endif