#ifndef TEXTURE_H
#define TEXTURE_H

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

/* Texture Library */
#include "soil.h"

/* C++ Includes */
#include <string>

class Texture {

	protected:

		/* Texture Name */
		string _name;
		/* Texture Filename */
		string _filename;

		/* OpenGL Texture Handler */
		GLuint _handler;
		/* OpenGL Texture Format (ex. GL_TEXTURE_2D) */
		GLenum _format;
		/* OpenGL Texture Shader Uniform */
		string _uniform;

	public:

		/* Constructors & Destructors */
		Texture(string name, GLuint format, string uniform, string fileName);
		Texture(string name, GLuint format, string uniform);

		~Texture();

		/* Loading Methods */
		virtual void loadTexture();
		virtual void loadUniforms(GLuint programID, GLuint textureID);

		/* Bind & Unbind to OpenGL Methods */
		virtual void bind(GLuint textureID);
		virtual void unbind(GLuint textureID);

		/* Getters */
		string getName();
		string getFilename();

		GLuint getHandler();
		GLenum getFormat();
		string getUniform();

		/* Setters */
		void setName(string name);
		void setFilename(string fileName);

		void setHandler(GLuint handler);
		void setFormat(GLenum format);
		void setUniform(string unifor);

		/* Debug Methods */
		void dump();
};

#endif