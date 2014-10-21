#include "PostProcessingEffect.h"

PostProcessingEffect::PostProcessingEffect(string name) {

	this->name = name;

	this->frameBuffer = new FrameBuffer();

	/* Create the quad for the post processing effect to be drawn to */
	GLfloat vertices[] = {	-1.0f, -1.0f, 
							 1.0f, -1.0f, 
							-1.0f,  1.0f, 
							 1.0f,  1.0f };

	glGenVertexArrays(1, &this->arrayObjectID);
	glBindVertexArray(this->arrayObjectID);

	glGenBuffers(1, &this->bufferObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, this->bufferObjectID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Utility::checkOpenGLError("ERROR: Could not bind the " + name + " Quad.");
}

PostProcessingEffect::~PostProcessingEffect() {

	delete this->frameBuffer;

	glDeleteBuffers(1, &this->bufferObjectID);
	glDeleteVertexArrays(1, &this->arrayObjectID);
}

void PostProcessingEffect::init(GLint width, GLint height) {

	this->frameBuffer->init(width, height);
}

string PostProcessingEffect::getName() {

	return this->name;
}

FrameBuffer* PostProcessingEffect::getFrameBuffer() {

	return this->frameBuffer;
}

GLuint PostProcessingEffect::getArrayObjectID() {

	return this->arrayObjectID;
}

GLuint PostProcessingEffect::getBufferObjectID() {

	return this->bufferObjectID;
}

void PostProcessingEffect::setName(string name) {

	this->name = name;
}

void PostProcessingEffect::setFrameBuffer(FrameBuffer* frameBuffer) {

	this->frameBuffer = frameBuffer;
}

void PostProcessingEffect::setArrayObjectID(GLuint arrayObjectID) {

	this->arrayObjectID = arrayObjectID;
}

void PostProcessingEffect::setBufferObjectID(GLuint vextexBufferObjectID) {

	this->bufferObjectID = vextexBufferObjectID;
}