#include "TableWall.h"

TableWall::TableWall(const char* name) : GraphicObject(name) {
}

TableWall::~TableWall() {
}

void TableWall::draw(GLuint programdID, GLuint uniformID) {

	//GraphicObject::draw(programdID,uniformID);
}

void TableWall::update(GLfloat elapsedTime) {

	GraphicObject::update(elapsedTime);
}

void TableWall::updateCollision() {

	GraphicObject::updateCollision();
}

void TableWall::dump() {

	cout << "<TableWall \"" << _name << "\" Dump>" << endl;

	/* Graphic Object Model Matrix */
	cout << "<GraphicObject> = " << endl;
	GraphicObject::dump();
}