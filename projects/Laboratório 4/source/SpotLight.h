#ifndef SPOT_LIGHT_H
#define	SPOT_LIGHT_H

#include "Light.h"

class SpotLight: public Light {

	private:

		Vector _direction;

		GLfloat _cutOff;

		GLfloat _constantAttenuation;
		GLfloat _linearAttenuation;
		GLfloat _exponentialAttenuation;

	public:

		/* Constructors & Destructors */
		SpotLight(const char* name);
		~SpotLight();

		/* Getters */
		Vector getDirection();

		GLfloat getCutOff();

		GLfloat getConstantAttenuation();
		GLfloat getLinearAttenuation();
		GLfloat getExponentinalAttenuation();

		/* Setters */
		void setDirection(Vector direction);

		void setCutOff(GLfloat cutOff);

		void setConstantAttenuation(GLfloat constantAttenuation);
		void setLinearAttenuation(GLfloat linearAttenuation);
		void setExponentialAttenuation(GLfloat exponentialAttenuation);

		void dump();
};

#endif