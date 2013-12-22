#pragma once

#include "GL/glew.h"
#include "GL/glut.h"

#include <fstream>
#include <climits>
#include <map>

#include "Utility.h"
#include "Constants.h"

#include "FrameBuffer.h"

#define VERTICES 0

#define TEXTURE_0_UNIFORM "Texture0"
#define TEXTURE_1_UNIFORM "Texture1"

#define POST_PROCESSING_VERTEX_SHADER_FILE		"shaders/post_proc_VertexShader.glsl"
#define POST_PROCESSING_FRAGMENT_SHADER_FILE	"shaders/post_proc_FragmentShader.glsl"

class PostProcessing {

	private:

		/* Shader Identifier */
		string _name;

		/* Shader Program File Paths */
		string _vertexShaderFile;
		string _fragmentShaderFile;

		/* Shader Program Code */
		const GLchar* _vertexShader;
		const GLchar* _fragmentShader;

		/* Shader Program ID */
		GLuint _programID;
		
		/* Shader Vertex & Fragment Program IDs */
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;

		/* Vertex Array IDs */
		GLuint _vertexArrayObjectID;
		GLuint _vertexBufferObjectID;

	public:

		/* Constructors & Destructors */
		PostProcessing(string name);
		~PostProcessing();

		/* GPU Creation & Destruction Methods */
		void createShaderProgram();

		void linkShaderProgram();

		virtual void bindAttributes();
		virtual void bindUniforms();

		void destroyShaderProgram();

		void bindAttributeLocation(GLuint index, string name);

		/* Scene Methods */
		void draw(FrameBuffer* frameBuffer);

		void reshape(FrameBuffer* frameBuffer);

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
};

