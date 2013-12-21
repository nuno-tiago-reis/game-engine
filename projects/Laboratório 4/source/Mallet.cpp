#include "Mallet.h"

Mallet::Mallet(const char* name) : GraphicObject(name) {
}

Mallet::~Mallet() {
}

void Mallet::update(GLfloat elapsedTime) {

	GraphicObject::update(elapsedTime);
}

void Mallet::updateCollision() {

	if(_boundingVolume->getCollidingVolumeType().compare("Bounding Box") == 0) {

		_position = _boundingVolume->getImpactPosition();
		_velocity = _boundingVolume->getImpactVelocity();
	}
	else if(_boundingVolume->getCollidingVolumeType().compare("Bounding Sphere") == 0) {

		Vector impactPosition = getBoundingVolume()->getImpactPosition();
		Vector impactVelocity = getBoundingVolume()->getImpactVelocity();

		Vector collisionPlaneN = getBoundingVolume()->getCollisionPlaneN();

		_position = impactPosition;
		//_velocity = Vector(0.0f,0.0f,0.0f,1.0f);
		_velocity = impactVelocity * _bounce + collisionPlaneN * _velocity.magnitude() * _bounce;
	}

	GraphicObject::updateCollision();
}

void Mallet::dump() {

	cout << "<Mallet \"" << _name << "\" Dump>" << endl;

	/* Graphic Object Model Matrix */
	cout << "<GraphicObject> = " << endl;
	GraphicObject::dump();
}