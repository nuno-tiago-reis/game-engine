#ifndef DIRECTIONAL_LIGHT_H
#define	DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight: public Light {

	private:

		Vector _direction;

	public:

		/* Constructors & Destructors */
		DirectionalLight(const char* name);
		~DirectionalLight();

		/* Getters */
		Vector getDirection();

		/* Setters */
		void setDirection(Vector direction);

		void dump();
};

#endif