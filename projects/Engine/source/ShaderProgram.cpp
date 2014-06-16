#include "ShaderProgram.h"

map<string,GLuint> ShaderProgram::_uniformBufferIndexMap;

bool ShaderProgram::bufferCreated=false;

ShaderProgram::ShaderProgram(string name, string vertexShaderFile, string fragmentShaderFile) {

	_name = name;

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

	_vertexShaderFile = vertexShaderFile;
	_fragmentShaderFile = fragmentShaderFile;

	_programID = UINT_MAX;

	_uniformBlockBindingPointMap[LIGHT_SOURCES_UNIFORM] = LIGHT_SOURCES_BINDING_POINT;
	_uniformBlockBindingPointMap[MATRICES_UNIFORM] = MATRICES_BINDING_POINT;

	_vertexShaderID = UINT_MAX;
	_fragmentShaderID = UINT_MAX;
}

ShaderProgram::~ShaderProgram() {

	destroyShaderProgram();

	delete _vertexShader;
	delete _fragmentShader;
}

void ShaderProgram::createShaderProgram() {

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

void ShaderProgram::linkShaderProgram() {

	/* Link Program Shader */
	glLinkProgram(_programID);

	/* Check if Program Linked Correctly */
	GLint shaderLinked;
	glGetProgramiv(_programID, GL_LINK_STATUS, &shaderLinked);

	if(shaderLinked == GL_FALSE)
		Utility::checkShaderLinkageError(_programID,_vertexShader,_vertexShaderFile.c_str(),_fragmentShader,_fragmentShaderFile.c_str());

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" linking error.");
}

void ShaderProgram::bindAttributes() {

	glBindAttribLocation(_programID, POSITION, POSITION_ATTRIBUTE);

	glBindAttribLocation(_programID, NORMAL, NORMAL_ATTRIBUTE);
	glBindAttribLocation(_programID, TANGENT, TANGENT_ATTRIBUTE);

	glBindAttribLocation(_programID, TEXTURE_UV, TEXTURE_UV_ATTRIBUTE);

	glBindAttribLocation(_programID, AMBIENT, AMBIENT_ATTRIBUTE);
	glBindAttribLocation(_programID, DIFFUSE, DIFFUSE_ATTRIBUTE);
	glBindAttribLocation(_programID, SPECULAR, SPECULAR_ATTRIBUTE);
	glBindAttribLocation(_programID, SHININESS, SHININESS_ATTRIBUTE);

	Utility::checkOpenGLError("ERROR: Shader program \"" + _name + "\" attribute binding error.");
}

void ShaderProgram::bindUniforms() {

	/* Initialize Uniforms */
	_uniformLocationMap["ModelMatrix"] = glGetUniformLocation(_programID, "ModelMatrix");

	/* Lights Block */
	_uniformBlockIndexMap[LIGHT_SOURCES_UNIFORM] = glGetUniformBlockIndex(_programID, LIGHT_SOURCES_UNIFORM);

	glUniformBlockBinding(_programID, _uniformBlockIndexMap[LIGHT_SOURCES_UNIFORM], _uniformBlockBindingPointMap[LIGHT_SOURCES_UNIFORM]);

	if(!bufferCreated) {

		unsigned char buffer[(sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * LIGHT_SOURCE_LIMIT];
		memset(buffer,0,(sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * LIGHT_SOURCE_LIMIT);

		GLuint uniformBufferIndex;
	
		glGenBuffers(1, &uniformBufferIndex);

		_uniformBufferIndexMap[LIGHT_SOURCES_UNIFORM] = uniformBufferIndex;

		glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferIndexMap[LIGHT_SOURCES_UNIFORM]);

		glBufferData(GL_UNIFORM_BUFFER, (sizeof(GLfloat) * LIGHT_SOURCE_STRUCT_SIZE + sizeof(GLint)) * LIGHT_SOURCE_LIMIT, buffer, GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_UNIFORM_BUFFER, _uniformBlockBindingPointMap[LIGHT_SOURCES_UNIFORM], _uniformBufferIndexMap[LIGHT_SOURCES_UNIFORM]);

		glBindBuffer(GL_UNIFORM_BUFFER,0);
	}

	/* Shared Matrices Block */
	_uniformBlockIndexMap[MATRICES_UNIFORM] = glGetUniformBlockIndex(_programID, MATRICES_UNIFORM);

	glUniformBlockBinding(_programID, _uniformBlockIndexMap[MATRICES_UNIFORM], _uniformBlockBindingPointMap[MATRICES_UNIFORM]);

	if(!bufferCreated) {
	
		GLuint uniformBufferIndex;

		glGenBuffers(1, &uniformBufferIndex);

		_uniformBufferIndexMap[MATRICES_UNIFORM] = uniformBufferIndex;

		glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferIndexMap[MATRICES_UNIFORM]);

		glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * MATRICES_STRUCT_SIZE, 0, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, _uniformBlockBindingPointMap[MATRICES_UNIFORM], _uniformBufferIndexMap[MATRICES_UNIFORM]);

		glBindBuffer(GL_UNIFORM_BUFFER,0);
	}

	bufferCreated=true;

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" uniform binding error.");
}

void ShaderProgram::destroyShaderProgram() {
	
	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" destruction error 11.");
	glUseProgram(0);

	cout << "[" << _name << "]" << " ProgramID = " << _programID << endl;
	cout << "[" << _name << "]" << " VertexProgramID = " << _vertexShaderID << endl;
	cout << "[" << _name << "]" << " FragmentProgramID = " << _fragmentShaderID << endl;

	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);

	glDeleteShader(_fragmentShaderID);
	glDeleteShader(_vertexShaderID);

	glDeleteProgram(_programID);

	Utility::checkOpenGLError("ERROR: Shader Program \"" + _name + "\" destruction error 22.");
}

string ShaderProgram::getName() {

	return _name;
}

string ShaderProgram::getVertexShaderFile() {

	return _vertexShaderFile;
}

string ShaderProgram::getFragmentShaderFile() {

	return _fragmentShaderFile;
}

const char* ShaderProgram::getVertexShader() {

	return _vertexShader;
}

const char* ShaderProgram::getFragmentShader() {

	return _fragmentShader;
}

GLuint ShaderProgram::getProgramID() {

	return _programID;
}

GLuint ShaderProgram::getUniformLocation(string uniformName) {

	return _uniformLocationMap[uniformName];
}

GLuint ShaderProgram::getUniformBlockIndex(string uniformBlockName) {

	return _uniformBlockIndexMap[uniformBlockName];
}

GLuint ShaderProgram::getUniformBlockBindingPoint(string uniformBlockName) {

	return _uniformBlockBindingPointMap[uniformBlockName];
}

GLuint ShaderProgram::getUniformBufferIndex(string uniformBlockName) {

	return _uniformBufferIndexMap[uniformBlockName];
}

GLuint ShaderProgram::getVertexShaderID() {

	return _vertexShaderID;
}

GLuint ShaderProgram::getFragmentShaderID() {

	return _fragmentShaderID;
}

void ShaderProgram::setName(string name) {

	_name = name;
}

void ShaderProgram::setVertexShaderFile(string vertexShaderFile) {

	_vertexShaderFile = vertexShaderFile;
}

void ShaderProgram::setFragmentShaderFile(string fragmentShaderFile) {

	_fragmentShaderFile = fragmentShaderFile;
}

void ShaderProgram::setVertexShader(const char* vertexShader) {

	_vertexShader = _strdup(vertexShader);
}

void ShaderProgram::setFragmentShader(const char* fragmentShader) {

	_fragmentShader = _strdup(fragmentShader);
}

void ShaderProgram::setProgramID(GLuint programID) {

	_programID = programID;
}

void ShaderProgram::setUniformLocation(string uniformName, GLuint uniformLocation) {

	_uniformLocationMap[uniformName] = uniformLocation;
}

void ShaderProgram::setUniformBlockIndex(string uniformName, GLuint uniformBlockIndex) {

	_uniformBlockIndexMap[uniformName] = uniformBlockIndex;
}

void ShaderProgram::setUniformBlockBindingPoint(string uniformBlockName, GLuint uniformBlockBindingPoint) {

	_uniformBlockBindingPointMap[uniformBlockName] = uniformBlockBindingPoint;
}

void ShaderProgram::setUniformBufferIndex(string uniformBlockName,  GLuint uniformBufferIndex) {

	_uniformBufferIndexMap[uniformBlockName] = uniformBufferIndex;
}

void ShaderProgram::setVertexShaderID(GLuint vertexShaderID) {

	_vertexShaderID = vertexShaderID;
}

void ShaderProgram::setFragmentShaderID(GLuint fragmentShaderID) {

	_fragmentShaderID = fragmentShaderID;
}

void ShaderProgram::dump() {

	cout << "<Shader Dump>" << endl;

	/* Shader Program File Paths */
	cout << "<Shader Vertex File Path> = " << _vertexShaderFile << " ;" << endl;
	cout << "<Shader Fragment File Path> = " << _fragmentShaderFile << " ;" << endl;

	/* Shader Program Code */
	//cout << "<Shader Vertex Code> = \n" << _vertexShader << " ;" << endl;
	//cout << "<Shader Fragment Code> = \n" << _fragmentShader << " ;" << endl;

	/* Shader Program ID */
	cout << "<Shader Program ID> = " << _programID << " ;" << endl;

	/* Shader Uniform Attributes */
	cout << "<Shader Uniform Locations> = " << endl;
	map<string,GLint>::const_iterator uniformLocationIterator;
	for(uniformLocationIterator = _uniformLocationMap.begin(); uniformLocationIterator != _uniformLocationMap.end(); uniformLocationIterator++)
		cout << "\t" << uniformLocationIterator->first << " = " << uniformLocationIterator->second << endl;

	cout << "<Shader Uniform Block Indexes> = " << endl;
	map<string,GLuint>::const_iterator uniformBlockIndexIterator;
	for(uniformBlockIndexIterator = _uniformBlockIndexMap.begin(); uniformBlockIndexIterator != _uniformBlockIndexMap.end(); uniformBlockIndexIterator++)
		cout << "\t" << uniformBlockIndexIterator->first << " = " << uniformBlockIndexIterator->second << endl;

	/* Shader Vertex & Fragment Program IDs */
	cout << "<Shader Vertex Program ID> = " << _vertexShaderID << " ;" << endl;
	cout << "<Shader Fragment Program ID> = " << _fragmentShaderID << " ;" << endl;
}