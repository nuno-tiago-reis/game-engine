#ifndef LIGHT_H
#define	LIGHT_H

#include <string>

#include "Vector.h"

#include "ShaderProgram.h"

#include "Utility.h"

#define LIGHT_SOURCE_0 0
#define LIGHT_SOURCE_1 1
#define LIGHT_SOURCE_2 2
#define LIGHT_SOURCE_3 3
#define LIGHT_SOURCE_4 4
#define LIGHT_SOURCE_5 5
#define LIGHT_SOURCE_6 6
#define LIGHT_SOURCE_7 7
#define LIGHT_SOURCE_8 8
#define LIGHT_SOURCE_9 9

class Light {

	protected:

		/* Light Name */
		string _name;

		/* Light Identifier (0-9) */
		GLint _identifier;

		/* Light Attributes */
		Vector _position;
		Vector _color;

		GLfloat _ambientIntensity;
		GLfloat _diffuseIntensity;
		GLfloat _specularIntensity;

		/* Shader Uniform Buffer Index */
		GLuint _uniformBufferIndex;

	public:

		/* Constructors & Destructors */
		Light(string name);
		~Light();

		/* Scene Methods */
		virtual void loadUniforms();

		/* Getters & Setters */
		string getName();

		GLint getIdentifier();

		Vector getPosition();
		Vector getColor();

		GLfloat getAmbientIntensity();
		GLfloat getDiffuseIntensity();
		GLfloat getSpecularIntensity();

		GLuint getUniformBufferIndex();

		/* Setters */
		void setName(string name);

		void setIdentifier(GLint identifier);

		void setPosition(Vector position);
		void setColor(Vector color);

		void setAmbientIntensity(GLfloat ambientIntensity);
		void setDiffuseIntensity(GLfloat diffuseIntensity);
		void setSpecularIntensity(GLfloat specularIntensity);

		void setUniformBufferIndex(GLuint uniformBufferIndex);

		void dump();
};

#endif