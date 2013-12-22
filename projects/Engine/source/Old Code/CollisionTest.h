#ifndef COLLISION_TEST_H
#define COLLISION_TEST_H

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "BoundingVolume.h"

#include "Vector.h"
#include "Constants.h"

#define A_SIDE 0
#define B_SIDE 1
#define C_SIDE 2
#define D_SIDE 3

using namespace std;

class CollisionTest {

	private:

		/* Singleton Instance */
		static CollisionTest *instance;

		/* Constructors & Destructors */
		CollisionTest();

		~CollisionTest();

	public:

		/* Singleton Methods */
		static CollisionTest* getInstance();
		static void destroyInstance();

		/* Collision Methods */
		bool isColliding(BoundingVolume* boundingVolumeA, BoundingVolume* boundingVolumeB);

		/* Swept Collision Based */

			/* Box-Box Collision */
			bool isColliding_SweptTest(BoundingBox* boundingBoxA, BoundingBox* boundingBoxB);

			/* Sphere-Sphere Collision */
			bool isColliding_SweptTest(BoundingSphere* boundingSphereA, BoundingSphere* boundingSphereB);

			/* Box-Sphere Collision */
			bool isColliding_SweptTest(BoundingSphere* boundingSphereA, BoundingBox* boundingBoxB);

		/* Line Segment Intersection */

			bool pointSegmentIntersection(Vector originA, Vector destinationA, Vector positionB);

			Vector segmentIntersection(Vector originA, Vector destinationA, Vector originB, Vector destinationB);

		/* Debug Methods */
		void dump();
};

#endif