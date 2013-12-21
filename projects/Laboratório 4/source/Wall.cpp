#include "Wall.h"

Wall::Wall(const char* name) : GraphicObject(name) {
}

Wall::~Wall() {
}

void Wall::update(GLfloat elapsedTime) {

	GraphicObject::update(elapsedTime);
}

void Wall::draw(GLuint programID, GLuint uniformID) {

	//do nothing. collision boxes shoud not be drawn
}

void Wall::updateCollision() {
}

void Wall::dump() {

	cout << "<Wall \"" << _name << "\" Dump>" << endl;

	/* Graphic Object Model Matrix */
	cout << "<GraphicObject> = " << endl;
	GraphicObject::dump();
}