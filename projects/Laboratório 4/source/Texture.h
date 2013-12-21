#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "soil.h"
#include "Utility.h"
#include <iostream>

class Texture {

	protected:

		string _fileName;

		GLuint _textureHandler;

		GLenum _textureFormat;

	public:

		Texture(GLuint textureFormat, const char* fname);

		void load();

		virtual void bind(GLuint textureID);
		void unbind(GLuint textureID);
};

#endif