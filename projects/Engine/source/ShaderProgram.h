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

/* Shared Matrices Uniforms */
#define MATRICES_UNIFORM "SharedMatrices"
#define MATRICES_BINDING_POINT 0
#define MATRICES_STRUCT_SIZE 32

/* Light Source Uniforms */
#define LIGHT_SOURCES_UNIFORM "LightSources"
#define LIGHT_SOURCES_BINDING_POINT 1
#define LIGHT_SOURCE_STRUCT_SIZE 19
#define LIGHT_SOURCE_LIMIT 10

/* Model Matrix Uniforms */
#define MODEL_MATRIX_UNIFORM "ModelMatrix"

/* Position Attributes */
#define POSITION 0
#define POSITION_ATTRIBUTE "Position"

#define TEXTURE_UV 1
#define TEXTURE_UV_ATTRIBUTE "TextureUV"

/* Normal Attributes */
#define NORMAL 2
#define NORMAL_ATTRIBUTE "Normal"
#define TANGENT 3
#define TANGENT_ATTRIBUTE "Tangent"

/* Color Attributes */
#define AMBIENT 4
#define AMBIENT_ATTRIBUTE "Ambient"
#define DIFFUSE 5
#define DIFFUSE_ATTRIBUTE "Diffuse"
#define SPECULAR 6
#define SPECULAR_ATTRIBUTE "Specular"
#define SPECULAR_CONSTANTS 7
#define SPECULAR_CONSTANT_ATTRIBUTE "SpecularConstant"

using namespace std;

class ShaderProgram {

	protected:

		/* Shader Identifier */
		string _name;

		/* Shader Program File Paths */
		string _vertexShaderFile;
		string _fragmentShaderFile;

		/* Shader Program Code */
		const char* _vertexShader;
		const char* _fragmentShader;

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
		ShaderProgram(string name, string vertexShaderFile, string fragmentShaderFile);
		~ShaderProgram();

		/* GPU Creation & Destruction Methods */
		void createShaderProgram();

		void linkShaderProgram();

		virtual void bindAttributes();
		virtual void bindUniforms();

		void destroyShaderProgram();

		/* Getters */
		string getName();

		string getVertexShaderFile();
		string getFragmentShaderFile();

		const char* getVertexShader();
		const char* getFragmentShader();

		GLuint getProgramID();

		GLuint getUniformLocation(string uniformName);

		GLuint getUniformBlockIndex(string uniformBlockName);
		GLuint getUniformBlockBindingPoint(string uniformBlockName);
		GLuint getUniformBufferIndex(string uniformBlockName);

		GLuint getVertexShaderID();
		GLuint getFragmentShaderID();

		/* Setters */
		void setName(string name);

		void setVertexShaderFile(string vertexShaderFile);
		void setFragmentShaderFile(string fragmentShaderFile);

		void setVertexShader(const char* vertexShader);
		void setFragmentShader(const char* fragmentShader);

		void setProgramID(GLuint programID);

		void setUniformLocation(string uniformName, GLuint uniformLocation);

		void setUniformBlockIndex(string uniformBlockName, GLuint uniformBlockIndex);
		void setUniformBlockBindingPoint(string uniformBlockName, GLuint uniformBlockBindingPoint);
		void setUniformBufferIndex(string uniformBlockName, GLuint uniformBufferIndex);

		void setVertexShaderID(GLuint vertexShaderID);
		void setFragmentShaderID(GLuint fragmentShaderID);

		/* Debug Methods */
		void dump();
};

#endif