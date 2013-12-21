#include "Puck.h"

Puck::Puck(const char* name) : GraphicObject(name) {

	_bounce = 0.9f;
	_friction = 0.99f;
}

Puck::~Puck() {
}

void Puck::update(GLfloat elapsedTime) {

	_velocity = _velocity * _friction;
	_velocity.clean();

	GraphicObject::update(elapsedTime);
}

void Puck::updateCollision() {

	if(_boundingVolume->getCollidingVolumeType().compare("Bounding Box") == 0) {

		Vector impactPosition = getBoundingVolume()->getImpactPosition();
		Vector impactVelocity = getBoundingVolume()->getImpactVelocity();

		Vector collisionPlaneN = getBoundingVolume()->getCollisionPlaneN();

		_position = impactPosition;
		_velocity = (collisionPlaneN * Vector::dotProduct(_velocity,collisionPlaneN) * -2 + _velocity ) * _bounce;
	}
	else if(_boundingVolume->getCollidingVolumeType().compare("Bounding Sphere") == 0) {

		Vector impactPosition = getBoundingVolume()->getImpactPosition();
		Vector impactVelocity = getBoundingVolume()->getImpactVelocity();

		Vector collisionPlaneN = getBoundingVolume()->getCollisionPlaneN();

		_position = impactPosition;
		_velocity = impactVelocity * _bounce + collisionPlaneN * _velocity.magnitude() * _bounce;

		if(_velocity.magnitude() > 25.0f)
			_velocity = _velocity * (25.0f / _velocity.magnitude());
	}

	GraphicObject::updateCollision();
}

void Puck::dump() {

	cout << "<Puck \"" << _name << "\" Dump>" << endl;

	/* Graphic Object Model Matrix */
	cout << "<GraphicObject> = " << endl;
	GraphicObject::dump();
}