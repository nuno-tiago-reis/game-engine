#include "Texture.h"

Texture::Texture(string name, GLuint format, string uniform, string filename) {

	/* Initialize the Textures Name */
	_name = name;

	/* Initialize the Texture Format (eg. GL_TEXTURE_2D) */
	_format = format;
	/* Initialize the Texture Uniform (specified by the Shader Program) */
	_uniform = uniform;

	/* Initialize the Textures Filname */
	_filename = filename;
}

Texture::Texture(string name, GLuint format, string uniform) {

	/* Initialize the Textures Name */
	_name = name;

	/* Texture Format (eg. GL_TEXTURE_2D) */
	_format = format;
	/* Texture Uniform - Specified by the Shader Program */
	_uniform = uniform;
}

Texture::~Texture() {
}

void Texture::loadTexture() {

	/* Load the Texture */
	_handler = SOIL_load_OGL_texture(_filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

	/* Check for an error during the load process */
	if(_handler == 0)
		cout << "SOIL loading error (\"" << _filename.c_str() << "\": " << SOIL_last_result() << std::endl;

	Utility::checkOpenGLError("ERROR: Texture \"" + _name + "\" loading failed.");
}

void Texture::loadUniforms(GLuint programID, GLuint textureID) {

	/* Load the Texture to the corresponding Uniform */
	glProgramUniform1i(programID, glGetUniformLocation(programID, _uniform.c_str()), textureID);

	Utility::checkOpenGLError("ERROR: Texture \"" + _name + "\" loading failed.");
}

void Texture::bind(GLuint textureID) {

	glActiveTexture(textureID);

    glBindTexture(_format, _handler);

	glTexParameteri(_format, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(_format, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(_format, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(_format, GL_TEXTURE_WRAP_T, GL_REPEAT);

	Utility::checkOpenGLError("ERROR: Texture \"" + _name + "\" binding failed.");
}

void Texture::unbind(GLuint textureID) {

	glBindTexture(_format, 0);

	Utility::checkOpenGLError("ERROR: Texture \"" + _name + "\" unbinding failed.");
}

string Texture::getName() {

	return _name;
}

string Texture::getFilename() {

	return _filename;
}

GLuint Texture::getHandler() {

	return _handler;
}

GLenum Texture::getFormat() {

	return _format;
}

string Texture::getUniform() {

	return _uniform;
}

void Texture::setName(string name) {

	_name = name;
}

void Texture::setFilename(string filename) {

	_filename = filename;
}

void Texture::setHandler(GLuint handler) {

	_handler = handler;
}

void Texture::setFormat(GLenum format) {

	_format = format;
}

void Texture::setUniform(string uniform) {

	_uniform = uniform;
}

void Texture::dump() {

	cout << "<Texture \"" << _name << "\" Dump>" << endl;

	/* Texture Filename */
	cout << "<Texture Filename> = " << _filename << endl;
	/* Texture Handler */
	cout << "<Texture Handler> = " << _handler << endl;
	/* Texture Format */
	cout << "<Texture Format> = " << _format << endl;
	/* Texture Uniform */
	cout << "<Texture Uniform> = " << _uniform << endl;
}