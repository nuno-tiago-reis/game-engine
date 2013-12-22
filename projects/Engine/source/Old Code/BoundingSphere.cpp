#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(GLfloat radius) {

	_type.assign(BOUNDING_SPHERE);

	_radius = radius;

	_position.setValue(0.0f,0.0f,0.0f,1.0f);
}

BoundingSphere::~BoundingSphere() {
}

void BoundingSphere::update(GraphicObject* graphicObject) {

	BoundingVolume::update(graphicObject);
}

void BoundingSphere::update(GraphicObject* graphicObject, GLfloat elapsedTime) {

	BoundingVolume::update(graphicObject,elapsedTime);
}

GLfloat BoundingSphere::getRadius() {

	return _radius;
}

void BoundingSphere::setRadius(GLfloat radius) {

	_radius = radius;
}

void BoundingSphere::dump() {

	cout << "<BoundingSphere Dump>" << endl;

	/* Sphere Radius */
	cout << "<BoundingSphere Radius> = " << _radius << " ;" << endl;

	BoundingVolume::dump();
}