#ifndef POSITIONAL_LIGHT_H
#define	POSITIONAL_LIGHT_H

#include "Light.h"

#define POSITIONAL_LIGHT 2

#define POSITIONAL_LIGHT_0 "Positional Light 0"
#define POSITIONAL_LIGHT_1 "Positional Light 1"
#define POSITIONAL_LIGHT_2 "Positional Light 2"
#define POSITIONAL_LIGHT_3 "Positional Light 3"
#define POSITIONAL_LIGHT_4 "Positional Light 4"
#define POSITIONAL_LIGHT_5 "Positional Light 5"
#define POSITIONAL_LIGHT_6 "Positional Light 6"
#define POSITIONAL_LIGHT_7 "Positional Light 7"
#define POSITIONAL_LIGHT_8 "Positional Light 8"
#define POSITIONAL_LIGHT_9 "Positional Light 9"

class PositionalLight: public Light {

	private:

		/* Positional Light Attributes */
		GLfloat _constantAttenuation;
		GLfloat _linearAttenuation;
		GLfloat _exponentialAttenuation;

	public:

		/* Constructors & Destructors */
		PositionalLight(string name);
		~PositionalLight();

		/* Scene Methods */
		void loadUniforms();

		/* Getters */
		GLfloat getConstantAttenuation();
		GLfloat getLinearAttenuation();
		GLfloat getExponentinalAttenuation();

		/* Setters */
		void setConstantAttenuation(GLfloat constantAttenuation);
		void setLinearAttenuation(GLfloat linearAttenuation);
		void setExponentialAttenuation(GLfloat exponentialAttenuation);

		void dump();
};

#endif