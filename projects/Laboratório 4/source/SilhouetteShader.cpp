#include "SilhouetteShader.h"

SilhouetteShader::SilhouetteShader(const char* name) :
	ShaderProgram(name,SILHOUETTE_VERTEX_SHADER_FILE,SILHOUETTE_FRAGMENT_SHADER_FILE) {
}

SilhouetteShader::~SilhouetteShader() {

	ShaderProgram::~ShaderProgram();
}

void SilhouetteShader::bindAttributes() {

	glBindAttribLocation(_programID, VERTICES, "Position");
	glBindAttribLocation(_programID, COLORS, "Color");

	Utility::checkOpenGLError("ERROR: Shader program \"" + _name + "\" attribute binding error.");
}

void SilhouetteShader::bindUniforms() {

	//Binds Projection and View Matrices
	ShaderProgram::bindUniforms();
}
