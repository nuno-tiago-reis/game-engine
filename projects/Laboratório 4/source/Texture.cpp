#include "Texture.h"

Texture::Texture(GLuint textureFormat, const char* fileName) {

	_fileName.assign(fileName);

	_textureFormat = textureFormat;
}

void Texture::load() {

	_textureHandler = SOIL_load_OGL_texture(_fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	/* check for an error during the load process */
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