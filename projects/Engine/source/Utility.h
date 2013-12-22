#ifndef UTILITY_H
#define UTILITY_H

#include "GL/glew.h"
#include "GL/glut.h"

#include "fmod_errors.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Utility {

	private:

		Utility();

		~Utility();

	public:

		static bool isOpenGLError() {

			bool isError = false;

			GLenum errCode;

			const GLubyte *errString;

			while ((errCode = glGetError()) != GL_NO_ERROR) {

				isError = true;

				errString = gluErrorString(errCode);

				cerr << "OpenGL ERROR [" << errString << "] = " << errCode << endl;
			}

			return isError;
		}

		static void checkOpenGLError(std::string error) {

			if(isOpenGLError()) {

				cerr << error << endl;

				exit(EXIT_FAILURE);
			}
		}

		static void checkShaderCompileError(GLuint shaderID, const char* file, const char* filename) {

			char *log;
			int logLength = 0;
			int charsWritten  = 0;

			cerr << "Shader Compilation ERROR [" << filename << "]" << endl;

			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH,&logLength);

			if(logLength > 0) {

				log = (char*)malloc(logLength);

				glGetShaderInfoLog(shaderID,logLength, &charsWritten,log);

				cerr <<	log << endl;

				cerr << file << endl;
			}	
		}

		static void checkShaderLinkageError(GLuint programID, const char* vertexShader, const char* vertexShaderFile, const char* fragmentShader , const char* fragmentShaderFile) {

			char *log;
			int logLength = 0;
			int charsWritten  = 0;

			cerr << "Shader Linkage ERROR [" << vertexShaderFile << "," << fragmentShaderFile << "]" << endl;

			glGetProgramiv(programID, GL_INFO_LOG_LENGTH,&logLength);

			if(logLength > 0) {

				log = (char*)malloc(logLength);

				glGetProgramInfoLog(programID, logLength, &charsWritten, log);

				cerr <<	log << endl;

				cerr << vertexShader << endl;

				cerr << fragmentShader << endl;
			}	
		}

		static void checkFMODError(FMOD_RESULT result) {

			if (result != FMOD_OK) {

				printf("FMOD error! (%d) %s\n",result,FMOD_ErrorString(result));
				exit(-1);
			}
		}
};
        

#endif