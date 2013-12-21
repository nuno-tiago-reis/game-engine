#ifndef LIGHTING_MANAGER_H
#define	LIGHTING_MANAGER_H

#include <map>

#include "SpotLight.h"
#include "PositionalLight.h"
#include "DirectionalLight.h"

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

#define SPOT_LIGHT 1
#define POSITIONAL_LIGHT 2
#define DIRECTIONAL_LIGHT 3

#define SPOT_LIGHT_0 "Spot Light 0"
#define SPOT_LIGHT_1 "Spot Light 1"

#define POSITIONAL_LIGHT_0 "Positional Light 0"
#define POSITIONAL_LIGHT_1 "Positional Light 1"
#define POSITIONAL_LIGHT_2 "Positional Light 2"

#define DIRECTIONAL_LIGHT_0 "Directional Light 0"

class LightingManager {

	private:

		/* Singleton Instance */
		static LightingManager* instance;

		/* Shader Uniform Buffer Index */
		GLuint _uniformBufferIndex;

		/* Spot Light Map  */
		map<string,SpotLight*> _spotLightMap;

		/* Positional Light Map  */
		map<string,PositionalLight*> _positionalLightMap;

		/* Directional Light Map  */
		map<string,DirectionalLight*> _directionalLightMap;

		/* Constructors & Destructors */
		LightingManager();
		~LightingManager();

	public:

		/* Singleton Methods */
		static LightingManager* getInstance();
		static void destroyInstance();

		/* Lighting Manager Methods */
		void loadUniforms();

		void loadSpotLight(SpotLight* spotLight);
		void loadPositionalLight(PositionalLight* positionalLight);
		void loadDirectionalLight(DirectionalLight* directionalLight);

		/* Spot Light Map Manipulation Methods */
		void addSpotLight(SpotLight* spotLight);
		void removeSpotLight(string spotLightName);

		SpotLight* getSpotLight(string spotLightName);

		/* Positional Light Map Manipulation Methods */
		void addPositionalLight(PositionalLight* positionalLight);
		void removePositionalLight(string positionalLightName);

		PositionalLight* getPositionalLight(string positionalLightName);

		/* Directional Light Map Manipulation Methods */
		void addDirectionalLight(DirectionalLight* directionalLight);
		void removeDirectionalLight(string directionalLightName);

		DirectionalLight* getDirectionalLight(string directionalLightName);

		/* Getters */
		GLuint getUniformBufferIndex();

		/* Setters */
		void setUniformBufferIndex(GLuint uniformBufferIndex);

		/* Debug Methods */
		void dump();
};

#endif