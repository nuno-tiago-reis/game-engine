#include "BoundingVolume.h"

BoundingVolume::BoundingVolume() {

	_position.setValue(0.0f, 0.0f, 0.0f, 1.0f);
	_impactPosition.setValue(0.0f, 0.0f, 0.0f, 1.0f);

	_velocity.setValue(0.0f, 0.0f, 0.0f, 1.0f);
	_impactVelocity.setValue(0.0f, 0.0f, 0.0f, 1.0f);

	_collisionPlaneX.setValue(0.0f, 0.0f, 0.0f, 1.0f);
	_collisionPlaneY.setValue(0.0f, 0.0f, 0.0f, 1.0f);
	_collisionPlaneN.setValue(0.0f, 0.0f, 0.0f, 1.0f);
}

BoundingVolume::~BoundingVolume() {
}

void BoundingVolume::update(GraphicObject* graphicObject) {

	Vector position = graphicObject->getPosition();
	Vector velocity = graphicObject->getVelocity();

	_position = position;
	_impactPosition.setValue(0.0f, 0.0f, 0.0f, 1.0f);

	_velocity = velocity;
	_impactVelocity.setValue(0.0f, 0.0f, 0.0f, 1.0f);

	_collisionPlaneX.setValue(0.0f, 0.0f, 0.0f, 1.0f);
	_collisionPlaneY.setValue(0.0f, 0.0f, 0.0f, 1.0f);
	_collisionPlaneN.setValue(0.0f, 0.0f, 0.0f, 1.0f);
}

void BoundingVolume::update(GraphicObject* graphicObject, GLfloat elapsedTime) {

	Vector position = graphicObject->getPosition();
	Vector velocity = graphicObject->getVelocity();

	_position = position;
	_impactPosition.setValue(0.0f, 0.0f, 0.0f, 1.0f);

	_velocity = velocity;
	_impactVelocity.setValue(0.0f, 0.0f, 0.0f, 1.0f);

	_elapsedTime = elapsedTime;

	_collisionPlaneX.setValue(0.0f, 0.0f, 0.0f, 1.0f);
	_collisionPlaneY.setValue(0.0f, 0.0f, 0.0f, 1.0f);
	_collisionPlaneN.setValue(0.0f, 0.0f, 0.0f, 1.0f);
}

string BoundingVolume::getType() {

	return _type;
}		

string BoundingVolume::getCollidingVolumeType() {

	return _collidingVolumeType;
}

Vector BoundingVolume::getPosition() {

	return _position;
}

Vector BoundingVolume::getImpactPosition() {

	return _impactPosition;
}

Vector BoundingVolume::getVelocity() {

	return _velocity;
}

Vector BoundingVolume::getImpactVelocity() {

	return _impactVelocity;
}

Vector BoundingVolume::getCollisionPlaneX() {

	return _collisionPlaneX;
}

Vector BoundingVolume::getCollisionPlaneY() {

	return _collisionPlaneY;
}

Vector BoundingVolume::getCollisionPlaneN() {

	return _collisionPlaneN;
}

GLfloat BoundingVolume::getElapsedTime() {

	return _elapsedTime;
}

void BoundingVolume::setType(const char* name) {

	_type.assign(name);
}

void BoundingVolume::setCollidingVolumeType(const char* name) {

	_collidingVolumeType.assign(name);
}

void BoundingVolume::setPosition(Vector position) {

	_position = position;
}

void BoundingVolume::setImpactPosition(Vector impactPosition) {

	_impactPosition = impactPosition;
}

void BoundingVolume::setVelocity(Vector velocity) {

	_velocity = velocity;
}

void BoundingVolume::setImpactVelocity(Vector impactVelocity) {

	_impactVelocity = impactVelocity;
}

void BoundingVolume::setCollisionPlaneX(Vector collisionPlaneX) {

	_collisionPlaneX = collisionPlaneX;
}

void BoundingVolume::setCollisionPlaneY(Vector collisionPlaneY) {

	_collisionPlaneY = collisionPlaneY;
}

void BoundingVolume::setCollisionPlaneN(Vector collisionPlaneN) {

	_collisionPlaneN = collisionPlaneN;
}

void  BoundingVolume::setElapsedTime(GLfloat elapsedTime) {

	_elapsedTime = elapsedTime;
}

void BoundingVolume::dump() {

	cout << "<BoundingVolume Dump>" << endl;

	/* Volume Type */
	cout << "<BoundingVolume Type> = " << _type << endl;
	cout << "<BoundingVolume Collision Type> = " << _collidingVolumeType << endl;

	/* Volume Position */
	cout << "<BoundingVolume Position> = " << endl;
	_position.dump();

	/* Volume Collision Plane */
	cout << "<BoundingVolume Collision Plane X> = " << endl;
	_collisionPlaneX.dump();

	cout << "<BoundingVolume Collision Plane Y> = " << endl;
	_collisionPlaneY.dump();

	cout << "<BoundingVolume Collision Plane N> = " << endl;
	_collisionPlaneN.dump();

	cout << "<BoundingVolume ElapsedTime> = " << _elapsedTime << endl;
}