#include "CollisionTest.h"

CollisionTest* CollisionTest::instance = NULL;

CollisionTest::CollisionTest() {
}

CollisionTest::~CollisionTest() {
}

CollisionTest* CollisionTest::getInstance() {

	if(instance == NULL)
		instance = new CollisionTest();

	return instance;
}

void CollisionTest::destroyInstance() {

	delete instance;

	instance = NULL;
}

bool CollisionTest::isColliding(BoundingVolume* boundingVolumeA, BoundingVolume* boundingVolumeB) {

	boundingVolumeA->setCollidingVolumeType("No Collision");
	boundingVolumeB->setCollidingVolumeType("No Collision");

	if(boundingVolumeA->getType().compare(BOUNDING_BOX) == 0 && boundingVolumeB->getType().compare(BOUNDING_BOX) == 0)
		return isColliding_SweptTest((BoundingBox*)boundingVolumeA,(BoundingBox*)boundingVolumeB);

	if(boundingVolumeA->getType().compare(BOUNDING_SPHERE) == 0 && boundingVolumeB->getType().compare(BOUNDING_SPHERE) == 0)
		return isColliding_SweptTest((BoundingSphere*)boundingVolumeA,(BoundingSphere*)boundingVolumeB);

	if(boundingVolumeA->getType().compare(BOUNDING_BOX) == 0 && boundingVolumeB->getType().compare(BOUNDING_SPHERE) == 0)
		return isColliding_SweptTest((BoundingSphere*)boundingVolumeB,(BoundingBox*)boundingVolumeA);

	if(boundingVolumeA->getType().compare(BOUNDING_SPHERE) == 0 && boundingVolumeB->getType().compare(BOUNDING_BOX) == 0)
		return isColliding_SweptTest((BoundingSphere*)boundingVolumeA,(BoundingBox*)boundingVolumeB);

	return false;
}

bool CollisionTest::isColliding_SweptTest(BoundingBox* boundingBoxA, BoundingBox* boundingBoxB) {

	if(boundingBoxA->getVelocity().magnitude() == 0 && boundingBoxB->getVelocity().magnitude() == 0)
		return false;

	GLfloat elapsedTime = boundingBoxA->getElapsedTime();

	/* Object A */
	Vector positionA = boundingBoxA->getPosition();
	Vector velocityA = boundingBoxA->getVelocity();

	/* Object B - Assumes B is stationary */
	Vector positionB = boundingBoxB->getPosition();
	Vector velocityB = boundingBoxB->getVelocity();

	/* Distances */
	Vector distance = positionB - positionA;

	/* Edge Normals, Corners A */
	Vector edgeNormalsA[4];
	
	edgeNormalsA[0] = boundingBoxA->getYAxis();
	edgeNormalsA[1] = boundingBoxA->getXAxis();
	edgeNormalsA[2] = -boundingBoxA->getYAxis();
	edgeNormalsA[3] = -boundingBoxA->getXAxis();

	Vector cornersA[4];

	for(int i=0; i<4; i++)
		cornersA[i] = boundingBoxA->getCorner(i);

	/* Edge Normals, Corners B */
	Vector edgeNormalsB[4];
	
	edgeNormalsB[0] = boundingBoxB->getYAxis();
	edgeNormalsB[1] = boundingBoxB->getXAxis();
	edgeNormalsB[2] = -boundingBoxB->getYAxis();
	edgeNormalsB[3] = -boundingBoxB->getXAxis();

	Vector cornersB[4];

	for(int i=0; i<4; i++)
		cornersB[i] = boundingBoxB->getCorner(i);

	Vector collisionPlaneN;

	GLfloat collisionTime = FLT_MIN;
	GLfloat leaveTime = FLT_MAX;

	/* Object A */
	for(int i=0; i<4; i++) {

		GLfloat minA = FLT_MAX, maxA = FLT_MIN;
		GLfloat minB = FLT_MAX, maxB = FLT_MIN;

		for(int j=0; j<4; j++) {

			GLfloat cornerProjection = Vector::dotProduct(cornersA[j],edgeNormalsA[i]);

			if(minA > cornerProjection)
				minA = cornerProjection;
			else if(maxA < cornerProjection)
				maxA = cornerProjection;

			cornerProjection = Vector::dotProduct(cornersB[j],edgeNormalsA[i]) + Vector::dotProduct(distance,edgeNormalsA[i]);

			if(minB > cornerProjection)
				minB = cornerProjection;
			else if(maxB < cornerProjection)
				maxB = cornerProjection;
		}

		GLfloat velocity = Vector::dotProduct(velocityA * elapsedTime,edgeNormalsA[i]);

		if(velocity < Vector::threshold)
			if(maxA < minB)
				return false;
			else 
				continue;

		GLfloat temporaryCollisionTime = (maxB - minA) / velocity;
		GLfloat temporaryLeaveTime = (minB - maxA) / velocity;

		if(temporaryCollisionTime > temporaryLeaveTime)
			swap(temporaryCollisionTime,temporaryLeaveTime);

		if(temporaryCollisionTime > 1.0f)
			return false;

		if(collisionTime < temporaryCollisionTime) {

			collisionPlaneN = -edgeNormalsA[i];
			collisionTime = temporaryCollisionTime;
		}
	}

	/* Object B */
	for(int i=0; i<4; i++) {

		GLfloat minA = FLT_MAX, maxA = FLT_MIN;
		GLfloat minB = FLT_MAX, maxB = FLT_MIN;

		for(int j=0; j<4; j++) {

			GLfloat cornerProjection = Vector::dotProduct(cornersA[j],edgeNormalsB[i]);

			if(minA > cornerProjection)
				minA = cornerProjection;
			else if(maxA < cornerProjection)
				maxA = cornerProjection;

			cornerProjection = Vector::dotProduct(cornersB[j],edgeNormalsB[i]) + Vector::dotProduct(distance,edgeNormalsB[i]);

			if(minB > cornerProjection)
				minB = cornerProjection;
			else if(maxB < cornerProjection)
				maxB = cornerProjection;
		}

		GLfloat velocity = Vector::dotProduct(velocityA * elapsedTime,edgeNormalsB[i]);

		if(velocity < Vector::threshold)
			if(maxA < minB)
				return false;
			else 
				continue;

		GLfloat temporaryCollisionTime = (maxB - minA) / velocity;
		GLfloat temporaryLeaveTime = (minB - maxA) / velocity;

		if(temporaryCollisionTime > temporaryLeaveTime)
			swap(temporaryCollisionTime,temporaryLeaveTime);

		if(temporaryCollisionTime > 1.0f)
			return false;

		if(collisionTime < temporaryCollisionTime) {

			collisionPlaneN = edgeNormalsA[i];
			collisionTime = temporaryCollisionTime;
		}
	}

	Vector impactPositionA = positionA + velocityA * elapsedTime * collisionTime * 0.25f;
	Vector impactVelocityA = Vector::projection(velocityA,Vector(collisionPlaneN[VY],-collisionPlaneN[VX],0.0f,1.0f));

	/* Update Object A Bounding Box */
	boundingBoxA->setCollisionPlaneN(collisionPlaneN);

	boundingBoxA->setImpactPosition(impactPositionA);
	boundingBoxA->setImpactVelocity(impactVelocityA);

	boundingBoxA->setCollidingVolumeType(boundingBoxB->getType().c_str());

	/* Update Object B Bounding Box */
	boundingBoxB->setCollisionPlaneN(-collisionPlaneN);

	boundingBoxB->setImpactPosition(positionB);
	boundingBoxB->setImpactVelocity(velocityB);

	boundingBoxB->setCollidingVolumeType(boundingBoxA->getType().c_str());

	return true;
}

bool CollisionTest::isColliding_SweptTest(BoundingSphere* boundingSphereA, BoundingSphere* boundingSphereB) {

	if(boundingSphereA->getVelocity().magnitude() == 0 && boundingSphereB->getVelocity().magnitude() == 0)
		return false;

	GLfloat elapsedTime = boundingSphereA->getElapsedTime();

	/* Object A */
	Vector positionA = boundingSphereA->getPosition();
	Vector velocityA = boundingSphereA->getVelocity();

	GLfloat radiusA = boundingSphereA->getRadius();

	/* Object B */
	Vector positionB = boundingSphereB->getPosition();
	Vector velocityB = boundingSphereB->getVelocity();

	GLfloat radiusB = boundingSphereB->getRadius();

	// Convert the circle for b into a 2D ray
	Vector rayOrigin = positionB;
	Vector rayDirection = velocityB * elapsedTime - velocityA * elapsedTime;

	Vector circleOrigin = positionA;
	GLfloat circleRadius = radiusA + radiusB;

    //CD_Circle c = new CD_Circle(a.Position, a.Radius + b.Radius);
	Vector origin = rayOrigin - circleOrigin;

	GLfloat a = Vector::dotProduct(rayDirection,rayDirection);
	GLfloat b = Vector::dotProduct(rayDirection, origin);
	GLfloat c = Vector::dotProduct(origin,origin)- circleRadius * circleRadius;

	// Early exit if:
	// - Ray origin outside circle (c > 0) AND 
	// - Ray pointing away from circle (b > 0)  
	if(b > 0.0f && c > 0.0f)
		return false;

	// Discriminant (Quadratic equation) = b^2 - 4 * a * c
	GLfloat discriminant = b * b - 4 * a * c;

	// A negative discriminant corresponds to Ray missing Sphere
	if(discriminant < 0.0f)
		return false;

	// Now Ray must hit Sphere
	// Compute smallest value of intersection (t)
	GLfloat collisionTime = (- b - sqrt(discriminant)) / (2 * a);

	// If (t) is negative, Ray started inside Sphere so clamp (t) to zero
	if(collisionTime < 0.0f)
		collisionTime = 0.0f;
       
	//contact.penetration = t;
	Vector collisionPoint = rayOrigin + rayDirection * collisionTime;
	Vector collisionPlaneN = -(collisionPoint - circleOrigin);
	collisionPlaneN.normalize();

	/* Update Object A Bounding Box */
	Vector impactPositionA = collisionPoint + collisionPlaneN * circleRadius;
	Vector impactVelocityA = collisionPlaneN * velocityB.magnitude();

	boundingSphereA->setCollisionPlaneN(collisionPlaneN);

	boundingSphereA->setImpactPosition(impactPositionA);
	boundingSphereA->setImpactVelocity(impactVelocityA);

	boundingSphereA->setCollidingVolumeType(boundingSphereB->getType().c_str());

	/* Update Object B Bounding Box */
	Vector impactPositionB = collisionPoint;
	Vector impactVelocityB = -collisionPlaneN * velocityA.magnitude();

	boundingSphereB->setCollisionPlaneN(-collisionPlaneN);

	boundingSphereB->setImpactPosition(impactPositionB);
	boundingSphereB->setImpactVelocity(impactVelocityB);

	boundingSphereB->setCollidingVolumeType(boundingSphereA->getType().c_str());

	return true;
}

bool CollisionTest::isColliding_SweptTest(BoundingSphere* boundingSphereA, BoundingBox* boundingBoxB) {

	if(boundingSphereA->getVelocity().magnitude() == 0 && boundingBoxB->getVelocity().magnitude() == 0)
		return false;

	GLfloat elapsedTime = boundingSphereA->getElapsedTime();

	/* Object A */
	Vector positionA = boundingSphereA->getPosition();
	Vector velocityA = boundingSphereA->getVelocity();
	Vector velocityAN = boundingSphereA->getVelocity();
	velocityAN.normalize();

	GLfloat radiusA = boundingSphereA->getRadius();

	/* Object B */
	Vector positionB = boundingBoxB->getPosition();
	Vector velocityB = boundingBoxB->getVelocity();

	Vector xAxisB = boundingBoxB->getXAxis();
	Vector yAxisB = boundingBoxB->getYAxis();

	GLfloat widthB = boundingBoxB->getWidth();
	GLfloat depthB = boundingBoxB->getDepth();

	Vector cornersB[4];
	
	cornersB[0] = boundingBoxB->getCorner(0) + positionB;
	cornersB[1] = boundingBoxB->getCorner(1) + positionB;
	cornersB[2] = boundingBoxB->getCorner(2) + positionB;
	cornersB[3] = boundingBoxB->getCorner(3) + positionB;

	/* Collision Point, Normal & Distance */
	Vector collisionPlaneN;
	Vector collisionPoint;
	GLfloat collisionDistance = FLT_MAX;

	Vector impactPositionA;
	Vector impactVelocityA;

	GLfloat sideDistance = FLT_MAX;

	/* Top Side Collision */
	Vector testSideDistance = segmentIntersection(positionA,positionB,cornersB[3],cornersB[0]);
	GLfloat testSideDistanceMagnitude = testSideDistance.magnitude();
	
	if(testSideDistanceMagnitude != 0.0f && sideDistance > testSideDistanceMagnitude) {

		sideDistance = testSideDistanceMagnitude;

		Vector origin = positionA - yAxisB * radiusA;
		Vector destination = positionA - yAxisB * radiusA + velocityA * elapsedTime;

		Vector testCollisionPoint = segmentIntersection(origin,destination,cornersB[3],cornersB[0]);
		GLfloat testCollisionPointMagnitude = (testCollisionPoint - origin).magnitude();

		if(collisionDistance > testCollisionPointMagnitude && testCollisionPoint.magnitude() != 0.0f) {

			collisionPlaneN = yAxisB;
			collisionPoint = testCollisionPoint;
			collisionDistance = testCollisionPointMagnitude;
		}
	}

	/* Right Side Collision */
	testSideDistance = segmentIntersection(positionA,positionB,cornersB[0],cornersB[1]);
	testSideDistanceMagnitude = testSideDistance.magnitude();
	
	if(testSideDistanceMagnitude != 0.0f && sideDistance > testSideDistanceMagnitude) {

		sideDistance = testSideDistanceMagnitude;

		Vector origin = positionA - xAxisB * radiusA;
		Vector destination = positionA - xAxisB * radiusA + velocityA * elapsedTime;

		Vector testCollisionPoint = segmentIntersection(origin,destination,cornersB[0],cornersB[1]);
		GLfloat testCollisionPointMagnitude = (testCollisionPoint - origin).magnitude();

		if(collisionDistance > testCollisionPointMagnitude && testCollisionPoint.magnitude() != 0.0f) {

			collisionPlaneN = xAxisB;
			collisionPoint = testCollisionPoint;
			collisionDistance = testCollisionPointMagnitude;
		}
	}

	/* Bottom Side Collision */
	testSideDistance = segmentIntersection(positionA,positionB,cornersB[1],cornersB[2]);
	testSideDistanceMagnitude = testSideDistance.magnitude();
	
	if(testSideDistanceMagnitude != 0.0f && sideDistance > testSideDistanceMagnitude) {

		sideDistance = testSideDistanceMagnitude;

		Vector origin = positionA + yAxisB * radiusA;
		Vector destination = positionA + yAxisB * radiusA + velocityA * elapsedTime;

		Vector testCollisionPoint = segmentIntersection(origin,destination,cornersB[1],cornersB[2]);
		GLfloat testCollisionPointMagnitude = (testCollisionPoint - origin).magnitude();

		if(collisionDistance > testCollisionPointMagnitude && testCollisionPoint.magnitude() != 0.0f) {

			collisionPlaneN = -yAxisB;
			collisionPoint = testCollisionPoint;
			collisionDistance = testCollisionPointMagnitude;
		}
	}

	/* Left Side Collision */
	testSideDistance = segmentIntersection(positionA,positionB,cornersB[2],cornersB[3]);
	testSideDistanceMagnitude = testSideDistance.magnitude();
	
	if(testSideDistanceMagnitude != 0.0f && sideDistance > testSideDistanceMagnitude) {

		sideDistance = testSideDistanceMagnitude;

		Vector origin = positionA + xAxisB * radiusA;
		Vector destination = positionA + xAxisB * radiusA + velocityA * elapsedTime;

		Vector testCollisionPoint = segmentIntersection(origin,destination,cornersB[2],cornersB[3]);
		GLfloat testCollisionPointMagnitude = (testCollisionPoint - origin).magnitude();

		if(collisionDistance > testCollisionPointMagnitude && testCollisionPoint.magnitude() != 0.0f) {

			collisionPlaneN = -xAxisB;
			collisionPoint = testCollisionPoint;
			collisionDistance = testCollisionPointMagnitude;
		}
	}

	if(collisionDistance == FLT_MAX) {

		Vector collisionCorner;

		if(cornersB[0][VX] < positionA[VX] && cornersB[0][VY] < positionA[VY])
			collisionCorner = cornersB[0];
		else if(cornersB[1][VX] < positionA[VX] && cornersB[1][VY] > positionA[VY])
			collisionCorner = cornersB[1];
		else if(cornersB[2][VX] > positionA[VX] && cornersB[2][VY] > positionA[VY])
			collisionCorner = cornersB[2];
		else if(cornersB[3][VX] > positionA[VX] && cornersB[3][VY] < positionA[VY])
			collisionCorner = cornersB[3];
		else
			return false;

		Vector rayOrigin = positionA;
		Vector rayDirection = velocityA * elapsedTime - velocityB * elapsedTime;

		Vector circleOrigin = collisionCorner;
		GLfloat circleRadius = 0.65f;

		Vector origin = rayOrigin - circleOrigin;

		GLfloat a = Vector::dotProduct(rayDirection,rayDirection);
		GLfloat b = Vector::dotProduct(rayDirection, origin);
		GLfloat c = Vector::dotProduct(origin,origin)- circleRadius * circleRadius;
 
		if(b > 0.0f && c > 0.0f)
			return false;

		GLfloat discriminant = b * b - 4 * a * c;

		if(discriminant < 0.0f)
			return false;

		GLfloat collisionTime = (- b - sqrt(discriminant)) / (2 * a);

		if(collisionTime < 0.0f)
			collisionTime = 0.0f;

		Vector collisionPoint = rayOrigin + rayDirection * collisionTime;
		Vector collisionPlaneN = collisionPoint - circleOrigin;
		collisionPlaneN.normalize();

		impactPositionA = collisionPoint + collisionPlaneN * 0.005f;
		impactVelocityA = collisionPlaneN;
	}
	else {

		collisionPlaneN.normalize();
	
		impactPositionA = collisionPoint + collisionPlaneN * radiusA * 1.1f;
		//impactVelocityA = Vector::projection(velocityA,Vector(collisionPlaneN[VY],-collisionPlaneN[VX],0.0f,1.0f));
		impactVelocityA = Vector::projection(velocityA,Vector(collisionPlaneN[VY],-collisionPlaneN[VX],0.0f,1.0f));
	}

	Vector impactPositionB = positionB;
	Vector impactVelocityB = velocityB;

	/* Update Object A Bounding Sphere */
	boundingSphereA->setImpactPosition(impactPositionA);
	boundingSphereA->setImpactVelocity(impactVelocityA);

	boundingSphereA->setCollisionPlaneN(collisionPlaneN);
	boundingSphereA->setCollidingVolumeType(boundingBoxB->getType().c_str());

	/* Update Object B Bounding Box */
	boundingBoxB->setImpactPosition(impactPositionB);
	boundingBoxB->setImpactVelocity(boundingSphereA->getVelocity());

	boundingBoxB->setCollisionPlaneN(-collisionPlaneN);
	boundingBoxB->setCollidingVolumeType(boundingSphereA->getType().c_str());

	return true;
}

void CollisionTest::dump() {

	cout << "<CollisionTest Dump>" << endl;
}

bool CollisionTest::pointSegmentIntersection(Vector originA, Vector destinationA, Vector positionB) {

    GLfloat m = (destinationA[VY] - originA[VY]) / (destinationA[VX] - originA[VX]);
    GLfloat b = originA[VY] - m * originA[VX];
   
    if(positionB[VY] == m * positionB[VX] + b)
		return true;

	return false;
}

Vector CollisionTest::segmentIntersection(Vector originA, Vector destinationA, Vector originB, Vector destinationB) {

	GLfloat ua = (destinationB[VX] - originB[VX]) * (originA[VY] - originB[VY]) - (destinationB[VY] - originB[VY]) * (originA[VX] - originB[VX]);
    GLfloat ub = (destinationA[VX] - originA[VX]) * (originA[VY] - originB[VY]) - (destinationA[VY] - originA[VY]) * (originA[VX] - originB[VX]);
    GLfloat denominator = (destinationB[VY] - originB[VY]) * (destinationA[VX] - originA[VX]) - (destinationB[VX] - originB[VX]) * (destinationA[VY] - originA[VY]);

	Vector intersectionPoint;

    bool intersection = false;
	bool coincident = false;

    if (abs(denominator) <= 0.00001f) {

        if (abs(ua) <= 0.00001f && abs(ub) <= 0.00001f) {

            intersection = coincident = true;

            intersectionPoint = (originA + destinationA) * 0.5;
			intersectionPoint[VZ] = 0.0f;
			intersectionPoint[VW] = 1.0f;

			return intersectionPoint;
        }
    }
    else {

        ua /= denominator;
        ub /= denominator;

        if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1) {

            intersection = true;
            intersectionPoint[VX] = originA[VX] + ua * (destinationA[VX] - originA[VX]);
            intersectionPoint[VY] = originA[VY] + ua * (destinationA[VY] - originA[VY]);
			intersectionPoint[VZ] = 0.0f;
			intersectionPoint[VW] = 1.0f;

			return intersectionPoint;
        }
    }

	return Vector(0.0f,0.0f,0.0f,1.0f);
}