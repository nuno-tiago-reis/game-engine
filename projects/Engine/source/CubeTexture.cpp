#include "CubeTexture.h"

CubeTexture::CubeTexture(string positiveXFileName, string negativeXFileName, string positiveYFileName,
			string negativeYFileName, string positiveZFileName,string negativeZFileName) {

	_textureFilenameMap[POSITIVE_X] = positiveXFileName;
	_textureFilenameMap[NEGATIVE_X] = negativeXFileName;

	_textureFilenameMap[POSITIVE_Y] = positiveYFileName;
	_textureFilenameMap[NEGATIVE_Y] = negativeYFileName;

	_textureFilenameMap[POSITIVE_Z] = positiveZFileName;
	_textureFilenameMap[NEGATIVE_Z] = negativeZFileName;

	_textureFormat = GL_TEXTURE_CUBE_MAP;
}

CubeTexture::CubeTexture(string fullTextureFileName) {

	_textureFilenameMap[FULL_TEXTURE] = fullTextureFileName;

	_textureFormat = GL_TEXTURE_CUBE_MAP;
}

CubeTexture::~CubeTexture() {
}

void CubeTexture::load() {

	if(_textureFilenameMap[FULL_TEXTURE].empty() == true) {

		/* Load the Texture */
		_textureHandler = SOIL_load_OGL_cubemap(
			_textureFilenameMap[POSITIVE_X].c_str(),_textureFilenameMap[NEGATIVE_X].c_str(),
			_textureFilenameMap[POSITIVE_Y].c_str(), _textureFilenameMap[NEGATIVE_Y].c_str(),
			_textureFilenameMap[POSITIVE_Z].c_str(), _textureFilenameMap[NEGATIVE_Z].c_str(),
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

		/* Check for an error during the load process */
		if(_textureHandler == 0)
			cout << "SOIL loading error (\"" << 
			_textureFilenameMap[POSITIVE_X].c_str() << _textureFilenameMap[NEGATIVE_X].c_str() << 
			_textureFilenameMap[POSITIVE_Y].c_str() << _textureFilenameMap[NEGATIVE_Y].c_str() <<
			_textureFilenameMap[POSITIVE_Z].c_str() << _textureFilenameMap[NEGATIVE_Z].c_str() <<
			"\": " << SOIL_last_result() << endl;

		Utility::checkOpenGLError("ERROR: Texture CubeMap loading failed.");
	}
	else {
	
		/* Load the Texture */
		_textureHandler = SOIL_load_OGL_single_cubemap(_textureFilenameMap[FULL_TEXTURE].c_str(),"NSWEUD",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

		/* Check for an error during the load process */
		if(_textureHandler == 0)
			cout << "SOIL loading error (\"" << _textureFilenameMap[FULL_TEXTURE].c_str() << "\": " << SOIL_last_result() << endl;

		Utility::checkOpenGLError("ERROR: Texture CubeMap loading failed.");
	}
}

void CubeTexture::bind(GLuint textureID) {

	glActiveTexture(textureID);

    glBindTexture(_textureFormat, _textureHandler);

	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	Utility::checkOpenGLError("ERROR: Texture \"" + _fileName + "\" binding failed.");
}

void CubeTexture::unbind(GLuint textureID) {

	glBindTexture(_textureFormat, 0);

	Utility::checkOpenGLError("ERROR: Texture \"" + _fileName + "\" unbinding failed.");
}