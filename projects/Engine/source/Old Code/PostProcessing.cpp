#include "PostProcessing.h"

PostProcessing::PostProcessing(string name) { // :
	//ShaderProgram(name,BUMPMAP_VERTEX_SHADER_FILE,BUMPMAP_FRAGMENT_SHADER_FILE) {{

	_name = name;

	ifstream fileStream;
	stringstream buffer;
	
	/* Read Vertex Shader File */
	string vertexShader;

	fileStream.open(POST_PROCESSING_VERTEX_SHADER_FILE,fstream::in);
	buffer << fileStream.rdbuf();
	vertexShader.append(buffer.str());

	/* Close the File Stream */
	fileStream.close();

	/* Clear the Buffer */
	buffer = stringstream();

	/* Read Fragment Shader File */
	string fragmentShader;

	fileStream.open(POST_PROCESSING_FRAGMENT_SHADER_FILE,fstream::in);
	buffer << fileStream.rdbuf();
	fragmentShader.append(buffer.str());

	/* Close the File Stream */
	fileStream.close();

	_vertexShader = _strdup(vertexShader.c_str());
	_fragmentShader = _strdup(fragmentShader.c_str());

	_vertexShaderFile = _strdup(POST_PROCESSING_VERTEX_SHADER_FILE);
	_fragmentShaderFile = _strdup(POST_PROCESSING_FRAGMENT_SHADER_FILE);

	/* Create Quad Vertices */
	GLfloat vertices[] = {	-1.0f, -1.0f, 1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f };

	glGenVertexArrays(1, &_vertexArrayObjectID);
	glBindVertexArray(_vertexArrayObjectID);

	glGenBuffers(1, &_vertexBufferObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Utility::checkOpenGLError("ERROR: Could not bind the Post Processing Quad.");
}

PostProcessing::~PostProcessing() {

	glDeleteBuffers(1, &_vertexBufferObjectID);
	glDeleteVertexArrays(1, &_vertexArrayObjectID);

	destroyShaderProgram();

	delete _vertexShader;
	delete _fragmentShader;
}

void PostProcessing::createShaderProgram() {

	/* Create Vertex Shader */
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	/* Compile Vertex Shader */
	glShaderSource(_vertexShaderID, 1, &_vertexShader, 0);
	glCompileShader(_vertexShaderID);

	/* Check if Vertex Shader Compiled */
	GLint shaderCompiled;
	glGetShaderiv(_vertexShaderID, GL_COMPILE_STATUS, &shaderCompiled);

	if(shaderCompiled == GL_FALSE)
		Utility::checkShaderCompileError(_vertexShaderID,_vertexShader,_vertexShaderFile.c_str());

	/* Create Fragment Shader */
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	/* Compile Fragment Shader */
	glShaderSource(_fragmentShaderID, 1, &_fragmentShader, 0);
	glCompileShader(_fragmentShaderID);

	/* Check if Fragment Shader Compiled */
	glGetShaderiv(_fragmentShaderID, GL_COMPILE_STATUS, &shaderCompiled);

	if(shaderCompiled == GL_FALSE)
		Utility::checkShaderCompileError(_fragmentShaderID,_fragmentShader,_fragmentShaderFile.c_str());

	/* Create Shader Program */
	_programID = glCreateProgram();

	/* Attach Vertex & Fragment Shader */
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" creation error.");
}

void PostProcessing::linkShaderProgram() {

	/* Link Program Shader */
	glLinkProgram(_programID);

	/* Check if Program Linked Correctly */
	GLint shaderLinked;
	glGetProgramiv(_programID, GL_LINK_STATUS, &shaderLinked);

	if(shaderLinked == GL_FALSE)
		Utility::checkShaderLinkageError(_programID,_vertexShader,_vertexShaderFile.c_str(),_fragmentShader,_fragmentShaderFile.c_str());

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" linking error.");
}

void PostProcessing::bindAttributes() {

	glBindAttribLocation(_programID, VERTICES, "VertexCoordinate");
}

void PostProcessing::bindUniforms() {

	/* Initialize Uniforms */
	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}

void PostProcessing::destroyShaderProgram() {

	glUseProgram(0);

	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);

	glDeleteShader(_fragmentShaderID);
	glDeleteShader(_vertexShaderID);

	glDeleteProgram(_programID);

	Utility::checkOpenGLError("ERROR: Post Processing Shader Program \"" + _name + "\" destruction error.");
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

string PostProcessing::getName() {

	return _name;
}

string PostProcessing::getVertexShaderFile() {

	return _vertexShaderFile;
}

string PostProcessing::getFragmentShaderFile() {

	return _fragmentShaderFile;
}

const char* PostProcessing::getVertexShader() {

	return _vertexShader;
}

const char* PostProcessing::getFragmentShader() {

	return _fragmentShader;
}

GLuint PostProcessing::getProgramID() {

	return _programID;
}

GLuint PostProcessing::getVertexShaderID() {

	return _vertexShaderID;
}

GLuint PostProcessing::getFragmentShaderID() {

	return _fragmentShaderID;
}

void PostProcessing::setName(string name) {

	_name = name;
}

void PostProcessing::setVertexShaderFile(string vertexShaderFile) {

	_vertexShaderFile = vertexShaderFile;
}

void PostProcessing::setFragmentShaderFile(string fragmentShaderFile) {

	_fragmentShaderFile = fragmentShaderFile;
}

void PostProcessing::setVertexShader(const char* vertexShader) {

	_vertexShader = _strdup(vertexShader);
}

void PostProcessing::setFragmentShader(const char* fragmentShader) {

	_fragmentShader = _strdup(fragmentShader);
}

void PostProcessing::setProgramID(GLuint programID) {

	_programID = programID;
}

void PostProcessing::setVertexShaderID(GLuint vertexShaderID) {

	_vertexShaderID = vertexShaderID;
}

void PostProcessing::setFragmentShaderID(GLuint fragmentShaderID) {

	_fragmentShaderID = fragmentShaderID;
}