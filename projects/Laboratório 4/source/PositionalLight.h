#ifndef POSITIONAL_LIGHT_H
#define	POSITIONAL_LIGHT_H

#include "Light.h"

class PositionalLight: public Light {

	private:

		GLfloat _constantAttenuation;
		GLfloat _linearAttenuation;
		GLfloat _exponentialAttenuation;

	public:

		/* Constructors & Destructors */
		PositionalLight(const char* name);
		~PositionalLight();

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