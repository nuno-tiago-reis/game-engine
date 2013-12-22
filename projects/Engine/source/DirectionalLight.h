#ifndef DIRECTIONAL_LIGHT_H
#define	DIRECTIONAL_LIGHT_H

#include "Light.h"

#define DIRECTIONAL_LIGHT 3

#define DIRECTIONAL_LIGHT_0 "Directional Light 0"
#define DIRECTIONAL_LIGHT_1 "Directional Light 1"
#define DIRECTIONAL_LIGHT_2 "Directional Light 2"
#define DIRECTIONAL_LIGHT_3 "Directional Light 3"
#define DIRECTIONAL_LIGHT_4 "Directional Light 4"
#define DIRECTIONAL_LIGHT_5 "Directional Light 5"
#define DIRECTIONAL_LIGHT_6 "Directional Light 6"
#define DIRECTIONAL_LIGHT_7 "Directional Light 7"
#define DIRECTIONAL_LIGHT_8 "Directional Light 8"
#define DIRECTIONAL_LIGHT_9 "Directional Light 9"

class DirectionalLight: public Light {

	private:

		/* Directional Light Attributes */
		Vector _direction;

	public:

		/* Constructors & Destructors */
		DirectionalLight(string name);
		~DirectionalLight();

		/* Scene Methods */
		void loadUniforms();

		/* Getters */
		Vector getDirection();

		/* Setters */
		void setDirection(Vector direction);

		void dump();
};

#endif