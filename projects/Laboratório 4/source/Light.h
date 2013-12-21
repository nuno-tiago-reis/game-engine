#ifndef LIGHT_H
#define	LIGHT_H

#include <string>

#include "Vector.h"

class Light {

	protected:

		string _name;

		GLint _identifier;

		Vector _position;
		Vector _color;

		GLfloat _ambientIntensity;
		GLfloat _diffuseIntensity;
		GLfloat _specularIntensity;

	public:

		/* Constructors & Destructors */
		Light(const char* name);
		~Light();

		/* Getters & Setters */
		string getName();

		GLint getIdentifier();

		Vector getPosition();
		Vector getColor();

		GLfloat getAmbientIntensity();
		GLfloat getDiffuseIntensity();
		GLfloat getSpecularIntensity();

		/* Setters */
		void setName(const char* name);

		void setIdentifier(GLint identifier);

		void setPosition(Vector position);
		void setColor(Vector color);

		void setAmbientIntensity(GLfloat ambientIntensity);
		void setDiffuseIntensity(GLfloat diffuseIntensity);
		void setSpecularIntensity(GLfloat specularIntensity);

		void dump();
};

#endif