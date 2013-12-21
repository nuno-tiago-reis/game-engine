#include "Pin.h"

Pin::Pin(const char* name) : GraphicObject(name) {
}

Pin::~Pin() {
}

void Pin::update(GLfloat elapsedTime) {

	GraphicObject::update(elapsedTime);
}

void Pin::updateCollision() {

	GraphicObject::updateCollision();
}

void Pin::dump() {

	cout << "<Pin \"" << _name << "\" Dump>" << endl;

	/* Graphic Object Model Matrix */
	cout << "<GraphicObject> = " << endl;
	GraphicObject::dump();
}