#ifndef SPOT_LIGHT_H
#define	SPOT_LIGHT_H

#include "Light.h"

#define SPOT_LIGHT 1

#define SPOT_LIGHT_0 "Spot Light 0"
#define SPOT_LIGHT_1 "Spot Light 1"
#define SPOT_LIGHT_2 "Spot Light 2"
#define SPOT_LIGHT_3 "Spot Light 3"
#define SPOT_LIGHT_4 "Spot Light 4"
#define SPOT_LIGHT_5 "Spot Light 5"
#define SPOT_LIGHT_6 "Spot Light 6"
#define SPOT_LIGHT_7 "Spot Light 7"
#define SPOT_LIGHT_8 "Spot Light 8"
#define SPOT_LIGHT_9 "Spot Light 9"

class SpotLight: public Light {

	private:

		/* Spot Light Attributes */
		Vector _direction;

		GLfloat _cutOff;

		GLfloat _constantAttenuation;
		GLfloat _linearAttenuation;
		GLfloat _exponentialAttenuation;

	public:

		/* Constructors & Destructors */
		SpotLight(string name);
		~SpotLight();

		/* Scene Methods */
		void loadUniforms();

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