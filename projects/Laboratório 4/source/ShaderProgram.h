#ifndef SHADER_H
#define SHADER_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"

#include <fstream>
#include <climits>

#include <map>

#include "Utility.h"
#include "Constants.h"

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

#define MATRICES_UNIFORM "SharedMatrices"
#define MATRICES_BINDING_POINT 0
#define MATRICES_STRUCT_SIZE 32

#define LIGHT_SOURCES_UNIFORM "LightSources"
#define LIGHT_SOURCES_BINDING_POINT 1
#define LIGHT_SOURCE_STRUCT_SIZE 19
#define LIGHT_SOURCE_LIMIT 10

using namespace std;

class ShaderProgram {

	protected:

		/* Shader Identifier */
		string _name;

		GLuint _identifier;

		/* Shader Program File Paths */
		const char* _vertexShaderFile;
		const char* _fragmentShaderFile;

		/* Shader Program Code */
		const GLchar* _vertexShader;
		const GLchar* _fragmentShader;

		/* Shader Program ID */
		GLuint _programID;

		/* Shader Vertex & Fragment Program IDs */
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;

		/* Shader Uniform Attributes */
		map<string,GLint> _uniformLocationMap;

		map<string,GLuint> _uniformBlockIndexMap;
		map<string,GLuint> _uniformBlockBindingPointMap;

		static bool bufferCreated;
		static map<string,GLuint> _uniformBufferIndexMap;

	public:

		/* Constructors & Destructors */
		ShaderProgram(const char* name, const char* vertexShaderFile, const char* fragmentShaderFile);
		~ShaderProgram();

		/* GPU Creation & Destruction Methods */
		void createShaderProgram();

		void linkShaderProgram();

		virtual void bindAttributes();
		virtual void bindUniforms();

		void destroyShaderProgram();

		void bindAttributeLocation(GLuint index, const char* name);

		/* Getters */
		string getName();

		GLuint getIdentifier();

		const char* getVertexShaderFile();
		const char* getFragmentShaderFile();

		const GLchar* getVertexShader();
		const GLchar* getFragmentShader();

		GLuint getProgramID();

		GLuint getUniformLocation(const char* uniformName);

		GLuint getUniformBlockIndex(const char* uniformBlockName);
		GLuint getUniformBlockBindingPoint(const char* uniformBlockName);
		GLuint getUniformBufferIndex(const char* uniformBlockName);

		GLuint getVertexShaderID();
		GLuint getFragmentShaderID();

		/* Setters */
		void setName(const char* name);

		void setIdentifier(GLuint identifier);

		void setVertexShaderFile(const char* vertexShaderFile);
		void setFragmentShaderFile(const char* fragmentShaderFile);

		void setVertexShader(const GLchar* vertexShader);
		void setFragmentShader(const GLchar* fragmentShader);

		void setProgramID(GLuint programID);

		void setUniformLocation(const char* uniformName, GLuint uniformLocation);

		void setUniformBlockIndex(const char* uniformBlockName, GLuint uniformBlockIndex);
		void setUniformBlockBindingPoint(const char* uniformBlockName, GLuint uniformBlockBindingPoint);
		void setUniformBufferIndex(const char* uniformBlockName, GLuint uniformBufferIndex);

		void setVertexShaderID(GLuint vertexShaderID);
		void setFragmentShaderID(GLuint fragmentShaderID);

		/* Debug Methods */
		void dump();
};

#endif