#include "PostProcessing.h"

PostProcessing* PostProcessing::instance = NULL;

PostProcessing::PostProcessing(string name) :
	ShaderProgram(name,POST_PROCESSING_VERTEX_SHADER_FILE,POST_PROCESSING_FRAGMENT_SHADER_FILE) {

	/* Create Quad Vertices */
	GLfloat vertices[] = {	-1.0f, -1.0f, 1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f };

	glGenVertexArrays(1, &_vertexArrayObjectID);
	glBindVertexArray(_vertexArrayObjectID);

	glGenBuffers(1, &_vertexBufferObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Utility::checkOpenGLError("ERROR: Could not bind the Post Processing Quad.");
}

PostProcessing::~PostProcessing() {

	glDeleteBuffers(1, &_vertexBufferObjectID);
	glDeleteVertexArrays(1, &_vertexArrayObjectID);

	ShaderProgram::~ShaderProgram();
}

PostProcessing* PostProcessing::getInstance() {

	if(instance == NULL)
		instance = new PostProcessing(POST_PROCESSING_SHADER);

	return instance;
}

void PostProcessing::destroyInstance() {

	delete instance;

	instance = NULL;
}

void PostProcessing::bindAttributes() {

	glBindAttribLocation(_programID, POSITION, POSITION_ATTRIBUTE);

	Utility::checkOpenGLError("ERROR: Shader program \"" + _name + "\" attribute binding error.");
}

void PostProcessing::bindUniforms() {

	/* Initialize Uniforms */
	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}

void PostProcessing::draw(FrameBuffer* frameBuffer) {

	/* Bind the Shader Program */
	glBindVertexArray(_vertexArrayObjectID);
	glUseProgram(_programID);

	/* Activate Texture 0 */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBuffer->getFrameBufferTexture(0));
	glUniform1i(glGetUniformLocation(_programID,TEXTURE_0_UNIFORM), 0);

	Utility::checkOpenGLError("ERROR: PostProcessing Texture 0 Binding error.");

	/* Activate Texture 1 */
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, frameBuffer->getFrameBufferTexture(1));
	glUniform1i(glGetUniformLocation(_programID,TEXTURE_1_UNIFORM), 1);

	Utility::checkOpenGLError("ERROR: PostProcessing Texture 1 Binding error.");

	/* Draw the Scene */
	glDisable(GL_DEPTH_TEST);

	glDrawArrays(GL_TRIANGLE_STRIP,0,4);

	glEnable(GL_DEPTH_TEST);

	/* Update Texture 1 */
	glBindTexture(GL_TEXTURE_2D, frameBuffer->getFrameBufferTexture(1));

	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,frameBuffer->getWidth(),frameBuffer->getHeight());

	glBindTexture(GL_TEXTURE_2D,0);

	Utility::checkOpenGLError("ERROR: Could not draw the post processing effect.");

	/* Unbind the Shader Program */
	glBindVertexArray(0);
	glUseProgram(0);
}

void PostProcessing::reshape(FrameBuffer* frameBuffer) {

	glBindTexture(GL_TEXTURE_2D, frameBuffer->getFrameBufferTexture(1));

	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer->getFrameBufferObject());

	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,frameBuffer->getWidth(),frameBuffer->getHeight());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D,0);
}

GLuint PostProcessing::getVertexArrayObjectID() {

	return _vertexArrayObjectID;
}

GLuint PostProcessing::getVertexBufferObjectID() {

	return _vertexBufferObjectID;
}

void PostProcessing::setVertexArrayObjectID(GLuint vertexArrayObjectID) {

	_vertexArrayObjectID = vertexArrayObjectID;
}

void PostProcessing::setVertexBufferObjectID(GLuint vextexBufferObjectID) {

	_vertexBufferObjectID = vextexBufferObjectID;
}