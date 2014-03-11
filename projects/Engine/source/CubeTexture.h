#ifndef CUBE_TEXTURE_H
#define CUBE_TEXTURE_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <map>

#include "Texture.h"

#include "soil.h"

#include <string>

#include "Utility.h"

#define POSITIVE_X "Positive X"
#define NEGATIVE_X "Negative X"

#define POSITIVE_Y "Positive Y"
#define NEGATIVE_Y "Negative Y"

#define POSITIVE_Z "Positive Z"
#define NEGATIVE_Z "Negative Z"

#define FULL_TEXTURE "Full Texture"

class CubeTexture : public Texture {

	private:

		/* Texture FileName */
		map<string,string> _textureFilenameMap;

	public:

		/* Constructors & Destructors */
		CubeTexture(string positiveXFileName, string negativeXFileName, string positiveYFileName,
			string negativeYFileName, string positiveZFileName,string negativeZFileName);
		CubeTexture(string fullTextureFileName);

		~CubeTexture();

		/* Loading Methods */
		void load();

		/* Bind & Unbind to OpenGL Methods */
		void bind(GLuint textureID);
		void unbind(GLuint textureID);
};

#endif