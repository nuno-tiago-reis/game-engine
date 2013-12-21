#pragma once

#include "GL/glew.h"
#include "GL/glut.h"

#include <fstream>
#include <climits>

#include "Utility.h"
#include "Constants.h"

class FrameBuffer
{
public:
	~FrameBuffer(void);
	static FrameBuffer* getInstance();
	void init(GLint width, GLint height);
	void destroy();
	void onReshape(GLint width, GLint height);
	GLint getFBO();
	GLint getFBOTexture();
	GLint getFBOTexture2();
	GLint getRBODepth();

	GLint getWidth();
	GLint getHeight();

//private:
	FrameBuffer(void);
	GLuint fbo, fbo_texture, rbo_depth, fbo_texture2;
	static FrameBuffer* instance;

	GLint w,h;
};

