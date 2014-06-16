#include "PostProcessingEffect.h"

PostProcessingEffect::PostProcessingEffect(string name) {

	_name = name;

	_frameBuffer = new FrameBuffer();

	/* Create the quad for the post processing effect to be drawn to */
	GLfloat vertices[] = {	-1.0f, -1.0f, 
							 1.0f, -1.0f, 
							-1.0f,  1.0f, 
							 1.0f,  1.0f };

	glGenVertexArrays(1, &_vertexArrayObjectID);
	glBindVertexArray(_vertexArrayObjectID);

	glGenBuffers(1, &_vertexBufferObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Utility::checkOpenGLError("ERROR: Could not bind the " + name + " Quad.");
}

PostProcessingEffect::~PostProcessingEffect() {

	delete _frameBuffer;

	glDeleteBuffers(1, &_vertexBufferObjectID);
	glDeleteVertexArrays(1, &_vertexArrayObjectID);
}

void PostProcessingEffect::init(GLint width, GLint height) {

	_frameBuffer->init(width, height);
}

string PostProcessingEffect::getName() {

	return _name;
}

FrameBuffer* PostProcessingEffect::getFrameBuffer() {

	return _frameBuffer;
}

GLuint PostProcessingEffect::getVertexArrayObjectID() {

	return _vertexArrayObjectID;
}

GLuint PostProcessingEffect::getVertexBufferObjectID() {

	return _vertexBufferObjectID;
}

void PostProcessingEffect::setName(string name) {

	_name = name;
}

void PostProcessingEffect::setFrameBuffer(FrameBuffer* frameBuffer) {

	_frameBuffer = frameBuffer;
}

void PostProcessingEffect::setVertexArrayObjectID(GLuint vertexArrayObjectID) {

	_vertexArrayObjectID = vertexArrayObjectID;
}

void PostProcessingEffect::setVertexBufferObjectID(GLuint vextexBufferObjectID) {

	_vertexBufferObjectID = vextexBufferObjectID;
}