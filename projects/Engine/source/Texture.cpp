#include "Texture.h"

Texture::Texture(GLuint textureFormat, string fileName) {

	_fileName = fileName;

	_textureFormat = textureFormat;
}

Texture::Texture(GLuint textureFormat) {

	_textureFormat = textureFormat;
}

Texture::Texture() {
}

Texture::~Texture() {
}

void Texture::load() {

	/* Load the Texture */
	_textureHandler = SOIL_load_OGL_texture(_fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

	/* Check for an error during the load process */
	if(_textureHandler == 0)
		cout << "SOIL loading error (\"" << _fileName.c_str() << "\": " << SOIL_last_result() << std::endl;

	Utility::checkOpenGLError("ERROR: Texture \"" + _fileName + "\" loading failed.");
}

void Texture::bind(GLuint textureID) {

	glActiveTexture(textureID);

    glBindTexture(_textureFormat, _textureHandler);

	glTexParameteri(_textureFormat, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(_textureFormat, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Utility::checkOpenGLError("ERROR: Texture \"" + _fileName + "\" binding failed.");
}

void Texture::unbind(GLuint textureID) {

	glBindTexture(_textureFormat, 0);

	Utility::checkOpenGLError("ERROR: Texture \"" + _fileName + "\" unbinding failed.");
}

string Texture::getFileName() {

	return _fileName;
}

GLuint Texture::getTextureHandler() {

	return _textureHandler;
}

GLenum Texture::getTextureFormat() {

	return _textureFormat;
}

void Texture::setFileName(string fileName) {

	_fileName = fileName;
}

void Texture::setTextureHandler(GLuint textureHandler) {

	_textureHandler = textureHandler;
}

void Texture::setTextureFormat(GLenum textureFormat) {

	_textureFormat = textureFormat;
}