#include "PostProcessing.h"


PostProcessing::PostProcessing(const char* name, const char* vertexShaderFile, const char* fragmentShaderFile){
	_name.assign(name);

	ifstream fileStream;
	stringstream buffer;
	
	/* Read Vertex Shader File */
	string vertexShader;

	fileStream.open(vertexShaderFile,fstream::in);
	buffer << fileStream.rdbuf();
	vertexShader.append(buffer.str());

	/* Close the File Stream */
	fileStream.close();

	/* Clear the Buffer */
	buffer = stringstream();

	/* Read Fragment Shader File */
	string fragmentShader;

	fileStream.open(fragmentShaderFile,fstream::in);
	buffer << fileStream.rdbuf();
	fragmentShader.append(buffer.str());

	/* Close the File Stream */
	fileStream.close();

	_vertexShader = _strdup(vertexShader.c_str());
	_fragmentShader = _strdup(fragmentShader.c_str());

	_vertexShaderFile = _strdup(vertexShaderFile);
	_fragmentShaderFile = _strdup(fragmentShaderFile);
	
	createProgram();
	initVertices();
	reset=true;
}

PostProcessing::~PostProcessing(void){
	deleteVertices();
	deleteProgram();
	delete _vertexShader;
	delete _fragmentShader;

	delete _vertexShaderFile;
	delete _fragmentShaderFile;
}

void PostProcessing::createProgram(){
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(_vertexShaderID, 1, &_vertexShader, 0);
	glCompileShader(_vertexShaderID);

	GLint shaderCompiled;

	/* Check if Vertex Shader Compiled */
	glGetShaderiv(_vertexShaderID, GL_COMPILE_STATUS, &shaderCompiled);

	if(shaderCompiled == GL_FALSE)
		Utility::checkShaderCompileError(_vertexShaderID,_vertexShader,_vertexShaderFile);

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragmentShaderID, 1, &_fragmentShader, 0);
	glCompileShader(_fragmentShaderID);

	/* Check if Fragment Shader Compiled */
	glGetShaderiv(_fragmentShaderID, GL_COMPILE_STATUS, &shaderCompiled);

	if(shaderCompiled == GL_FALSE)
		Utility::checkShaderCompileError(_fragmentShaderID,_fragmentShader,_fragmentShaderFile);

	program_postproc = glCreateProgram();
	glAttachShader(program_postproc, _vertexShaderID);
	glAttachShader(program_postproc, _fragmentShaderID);

	glLinkProgram(program_postproc);
	GLint shaderLinked;

	/* Check if Fragment Shader Compiled */
	glGetProgramiv(program_postproc, GL_LINK_STATUS, &shaderLinked);

	if(shaderLinked == GL_FALSE)
		Utility::checkShaderLinkageError(program_postproc,_vertexShader,_vertexShaderFile,_fragmentShader,_fragmentShaderFile);

	glBindAttribLocation(program_postproc,VCOORD,"v_coord");

	uniform_name = "fbo_texture";
	uniform_fbo_texture = glGetUniformLocation(program_postproc, uniform_name);
	uniform_name2 = "fbo_texture2";
	uniform_fbo_texture2 = glGetUniformLocation(program_postproc, uniform_name2);
}

void PostProcessing::deleteProgram(){
	glUseProgram(0);
	glDetachShader(program_postproc, _vertexShaderID);
	glDetachShader(program_postproc, _fragmentShaderID);

	glDeleteShader(_fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteProgram(program_postproc);
}

void PostProcessing::draw(FrameBuffer* fb){
	if(reset){
		glBindTexture(GL_TEXTURE_2D, fb->getFBOTexture2());
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fb->getFBO());
		glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,fb->getWidth(),fb->getHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D,0);
		reset=false;
	}

	glBindVertexArray(vao_fbo_vertices);
	glUseProgram(program_postproc);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fb->getFBOTexture());
	glUniform1i(uniform_fbo_texture, /*GL_TEXTURE*/0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fb->getFBOTexture2());
	glUniform1i(uniform_fbo_texture2, /*GL_TEXTURE*/1);

	Utility::checkOpenGLError("ERROR: could not bind the texture.");
	
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, fb->getFBOTexture2());
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,fb->getWidth(),fb->getHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D,0);

	Utility::checkOpenGLError("ERROR: Could not draw the post processing effect.");
	glBindVertexArray(0);
	glUseProgram(0);
}

void PostProcessing::fakeDraw(FrameBuffer* fb){
	glBindTexture(GL_TEXTURE_2D, fb->getFBOTexture2());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fb->getFBO());
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,fb->getWidth(),fb->getHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D,0);

	glBindVertexArray(vao_fbo_vertices);
	glUseProgram(program_postproc);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fb->getFBOTexture());
	glUniform1i(uniform_fbo_texture, /*GL_TEXTURE*/0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fb->getFBOTexture2());
	glUniform1i(uniform_fbo_texture2, /*GL_TEXTURE*/1);

	Utility::checkOpenGLError("ERROR: could not bind the texture.");
	
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	glEnable(GL_DEPTH_TEST);

	Utility::checkOpenGLError("ERROR: Could not draw the post processing effect.");
	glBindVertexArray(0);
	glUseProgram(0);
}

void PostProcessing::initVertices(){
	GLfloat fbo_vertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f
	};
	glGenVertexArrays(1, &vao_fbo_vertices);
	glBindVertexArray(vao_fbo_vertices);

	glGenBuffers(1, &vbo_fbo_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VCOORD);
	glVertexAttribPointer(VCOORD, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Utility::checkOpenGLError("ERROR: Could not bind the post processing quad.");
}

void PostProcessing::deleteVertices(){
	glDeleteBuffers(1, &vbo_fbo_vertices);
}

void PostProcessing::onCameraReset(){
	reset=true;
}