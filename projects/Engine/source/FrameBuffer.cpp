#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {

	this->width = 0;
	this->height = 0;

	this->frameBufferObject = 0;

	this->frameBufferTexture[0] = 0;
	this->frameBufferTexture[1] = 0;

	this->renderBufferObject = 0;
}

FrameBuffer::~FrameBuffer() {

	glDeleteRenderbuffers(1, &this->renderBufferObject);

	glDeleteTextures(1, &this->frameBufferTexture[0]);
	glDeleteTextures(1, &this->frameBufferTexture[1]);

	glDeleteFramebuffers(1, &this->frameBufferObject);
		
	Utility::checkOpenGLError("Failed to destroy FrameBuffer");
}

void FrameBuffer::init(GLint width, GLint height) {

	this->width = width;
	this->height = height;

	/* Framebuffer to link everything together */
	glGenFramebuffers(1, &this->frameBufferObject);

	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);

	Utility::checkOpenGLError("Failed to create FrameBuffer");

	/* First Texture */
	glGenTextures(1, &this->frameBufferTexture[0]);

	glBindTexture(GL_TEXTURE_2D, this->frameBufferTexture[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->frameBufferTexture[0], 0);

	Utility::checkOpenGLError("ERROR: Failed to create FrameBuffer Texture 0");

	/* Second Texture for the Motion Blur*/
	glGenTextures(1, &this->frameBufferTexture[1]);

	glBindTexture(GL_TEXTURE_2D, this->frameBufferTexture[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	Utility::checkOpenGLError("ERROR: Failed to create FrameBuffer Texture 1");

	/* Depth buffer */
	glGenRenderbuffers(1, &this->renderBufferObject);

	glBindRenderbuffer(GL_RENDERBUFFER, this->renderBufferObject);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->renderBufferObject);

	Utility::checkOpenGLError("ERROR: Failed to create FrameBuffer RenderBuffer");

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::reshape(GLint width, GLint height) {

	this->width = width;
	this->height = height;

	/* First Texture */
	glBindTexture(GL_TEXTURE_2D, this->frameBufferTexture[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	/* Second Texture for the Motion Blur*/
	glBindTexture(GL_TEXTURE_2D, this->frameBufferTexture[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
 
	/* Depth buffer */
	glBindRenderbuffer(GL_RENDERBUFFER, this->renderBufferObject);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLint FrameBuffer::getWidth() {

	return this->width;
}

GLint FrameBuffer::getHeight() {

	return this->height;
}

GLuint FrameBuffer::getFrameBufferObject() {

	return this->frameBufferObject;
}

GLuint FrameBuffer::getFrameBufferTexture(int index) {

	return this->frameBufferTexture[index];
}

GLuint FrameBuffer::getRenderBufferObject() {

	return this->renderBufferObject;
}

void FrameBuffer::setWidth(GLint width) {

	this->width = width;
}

void FrameBuffer::setHeight(GLint height) {

	this->height = height;
}

void FrameBuffer::setFrameBufferObject(GLuint frameBufferObjectHandler) {

	this->frameBufferObject = frameBufferObjectHandler;
}

void FrameBuffer::setFrameBufferTexture(GLuint frameBufferTextureHandler, int index) {

	this->frameBufferTexture[index] = frameBufferTextureHandler;
}

void FrameBuffer::setRenderBufferObject(GLuint renderBufferObject) {

	this->renderBufferObject = renderBufferObject;
}