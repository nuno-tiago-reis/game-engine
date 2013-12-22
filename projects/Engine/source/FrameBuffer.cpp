#include "FrameBuffer.h"

FrameBuffer* FrameBuffer::instance = NULL;

FrameBuffer::FrameBuffer() {

	_width = 0;
	_height = 0;

	_frameBufferObject = 0;
	_frameBufferTexture[0] = 0;
	_frameBufferTexture[1] = 0;

	_renderBufferObject = 0;
}

FrameBuffer::~FrameBuffer() {
}

FrameBuffer* FrameBuffer::getInstance() {

	if(instance == NULL) 
		instance = new FrameBuffer();

	return instance;
}

void FrameBuffer::destroyInstance() {

	if(instance != NULL) {

		GLuint frameBufferObject = instance->getFrameBufferObject();
		GLuint frameBufferTexture[] = { instance->getFrameBufferTexture(0), instance->getFrameBufferTexture(1) };

		GLuint renderBufferObject = instance->getRenderBufferObject();

		glDeleteRenderbuffers(1, &renderBufferObject);

		glDeleteTextures(1, &frameBufferTexture[0]);
		glDeleteTextures(1, &frameBufferTexture[1]);

		glDeleteFramebuffers(1, &frameBufferObject);
		
		Utility::checkOpenGLError("Failed to destroy FrameBuffer");
	}

	delete instance;

	instance = NULL;
}

void FrameBuffer::init(GLint width, GLint height) {

	_width = width;
	_height = height;

	/* Framebuffer to link everything together */
	glGenFramebuffers(1, &_frameBufferObject);

	glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);

	Utility::checkOpenGLError("Failed to create FrameBuffer");

	/* First Texture */
	glGenTextures(1, &_frameBufferTexture[0]);

	glBindTexture(GL_TEXTURE_2D, _frameBufferTexture[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _frameBufferTexture[0], 0);

	Utility::checkOpenGLError("ERROR: Failed to create FrameBuffer Texture 0");

	/* Second Texture for the Motion Blur*/
	glGenTextures(1, &_frameBufferTexture[1]);

	glBindTexture(GL_TEXTURE_2D, _frameBufferTexture[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	Utility::checkOpenGLError("ERROR: Failed to create FrameBuffer Texture 1");

	/* Depth buffer */
	glGenRenderbuffers(1, &_renderBufferObject);

	glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferObject);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderBufferObject);

	Utility::checkOpenGLError("ERROR: Failed to create FrameBuffer RenderBuffer");

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::reshape(GLint width, GLint height) {

	_width = width;
	_height = height;

	/* First Texture */
	glBindTexture(GL_TEXTURE_2D, _frameBufferTexture[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	/* Second Texture for the Motion Blur*/
	glBindTexture(GL_TEXTURE_2D, _frameBufferTexture[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
 
	/* Depth buffer */
	glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferObject);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLint FrameBuffer::getWidth() {

	return _width;
}

GLint FrameBuffer::getHeight() {

	return _height;
}

GLuint FrameBuffer::getFrameBufferObject() {

	return _frameBufferObject;
}

GLuint FrameBuffer::getFrameBufferTexture(int index) {

	return _frameBufferTexture[index];
}

GLuint FrameBuffer::getRenderBufferObject() {

	return _renderBufferObject;
}

void FrameBuffer::setWidth(GLint width) {

	_width = width;
}

void FrameBuffer::setHeight(GLint height) {

	_height = height;
}

void FrameBuffer::setFrameBufferObject(GLuint frameBufferObjectHandler) {

	_frameBufferObject = frameBufferObjectHandler;
}

void FrameBuffer::setFrameBufferTexture(GLuint frameBufferTextureHandler, int index) {

	_frameBufferTexture[index] = frameBufferTextureHandler;
}

void FrameBuffer::setRenderBufferObject(GLuint renderBufferObject) {

	_renderBufferObject = renderBufferObject;
}