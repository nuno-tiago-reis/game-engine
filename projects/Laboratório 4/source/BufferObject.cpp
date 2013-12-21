#include "BufferObject.h"

BufferObject::BufferObject(const Vertex* vertices, GLint vertexCount) {

	_vertices = new Vertex[vertexCount];

	_vertexCount = vertexCount;

	memcpy(_vertices,vertices,sizeof(Vertex)*vertexCount);
}

BufferObject::~BufferObject() {

	delete _vertices;
}

void BufferObject::createBufferObject() {

	glGenVertexArrays(1, &_vertexArrayObjectID);
	glBindVertexArray(_vertexArrayObjectID);

	glGenBuffers(1, &_vertexBufferObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*_vertexCount, _vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(COLORS);
	glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(_vertices[0].XYZW));

	glEnableVertexAttribArray(NORMALS);
	glVertexAttribPointer(NORMALS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(_vertices[0].XYZW)+sizeof(_vertices[0].RGBA)));

	glEnableVertexAttribArray(TEXUVS);
	glVertexAttribPointer(TEXUVS, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(_vertices[0].XYZW)+sizeof(_vertices[0].RGBA)+sizeof(_vertices[0].NORMAL)));

	glEnableVertexAttribArray(AMBIENTS);
	glVertexAttribPointer(AMBIENTS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(_vertices[0].XYZW)+sizeof(_vertices[0].RGBA)+sizeof(_vertices[0].NORMAL)+sizeof(_vertices[0].TEXUV)));

	glEnableVertexAttribArray(TANGENTS);
	glVertexAttribPointer(TANGENTS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(_vertices[0].XYZW)+sizeof(_vertices[0].RGBA)+sizeof(_vertices[0].NORMAL)+sizeof(_vertices[0].TEXUV)+sizeof(_vertices[0].AMBIENT)));

	glEnableVertexAttribArray(SPECULARS);
	glVertexAttribPointer(SPECULARS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(_vertices[0].XYZW)+sizeof(_vertices[0].RGBA)+sizeof(_vertices[0].NORMAL)+sizeof(_vertices[0].TEXUV)+sizeof(_vertices[0].AMBIENT)+sizeof(_vertices[0].TANGENT)));

	glEnableVertexAttribArray(SPECULARS_CONSTANTS);
	glVertexAttribPointer(SPECULARS_CONSTANTS, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(_vertices[0].XYZW)+sizeof(_vertices[0].RGBA)+sizeof(_vertices[0].NORMAL)+sizeof(_vertices[0].TEXUV)+sizeof(_vertices[0].AMBIENT)+sizeof(_vertices[0].TANGENT)+sizeof(_vertices[0].SPECULAR)));

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDisableVertexAttribArray(NORMALS);
	glDisableVertexAttribArray(TEXUVS);
	glDisableVertexAttribArray(AMBIENTS);
	glDisableVertexAttribArray(TANGENTS);
	glDisableVertexAttribArray(SPECULARS);
	glDisableVertexAttribArray(SPECULARS_CONSTANTS);

	Utility::checkOpenGLError("ERROR: Buffer Object creation failed.");
}

void BufferObject::destroyBufferObject() {

	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDisableVertexAttribArray(NORMALS);
	glDisableVertexAttribArray(TEXUVS);
	glDisableVertexAttribArray(AMBIENTS);
	glDisableVertexAttribArray(TANGENTS);
	glDisableVertexAttribArray(SPECULARS);
	glDisableVertexAttribArray(SPECULARS_CONSTANTS);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &_vertexBufferObjectID);
	glDeleteVertexArrays(1, &_vertexArrayObjectID);

	Utility::checkOpenGLError("ERROR: Buffer Object destruction failed.");
}

GLint BufferObject::getVertexCount() {

	return _vertexCount;
}

Vertex* BufferObject::getVertices() {

	return _vertices;
}

GLuint BufferObject::getVertexArrayObjectID() {

	return _vertexArrayObjectID;
}

GLuint BufferObject::getVertexBufferObjectID() {

	return _vertexBufferObjectID;
}

void BufferObject::setVertexCount(GLint vertexCount) {

	_vertexCount = vertexCount;
}

void BufferObject::setVertices(Vertex* vertices, GLint vertexCount) {

	_vertexCount = vertexCount;

	memcpy(_vertices,vertices,sizeof(Vertex)*vertexCount);
}

void BufferObject::setVertexArrayObjectID(GLuint vertexArrayObjectID) {

	_vertexArrayObjectID = vertexArrayObjectID;
}

void BufferObject::setVertexBufferObjectID(GLuint vertexBufferObjectID) {

	_vertexBufferObjectID = vertexBufferObjectID;
}

void BufferObject::dump() {

	cout << "<BufferObject Dump>" << endl;

	/* Buffer Object Vertex Attributes */
	cout << "<BufferObject Vertex Count> = " << _vertexCount << " ;" << endl;

	cout << "<BufferObject Vertex List> = "  << endl;
	for(int i=0; i < _vertexCount; i++) {

		cout << "\tVertex " << i << " XYZW: ";
		cout << "\t[" << _vertices[i].RGBA[0] << "][" << _vertices[i].RGBA[1] << "][" << _vertices[i].RGBA[2] << "][" << _vertices[i].RGBA[3] << "]" << endl;

		cout << "\tVertex " << i << " RGBA: ";
		cout << "\t[" << _vertices[i].XYZW[0] << "][" << _vertices[i].XYZW[1] << "][" << _vertices[i].XYZW[2] << "][" << _vertices[i].XYZW[3] << "]" << endl;
	}

	/* Buffer Object OpenGL IDs */
	cout << "<BufferObject VertexArrayObject ID> = " << _vertexArrayObjectID << " ;" << endl;
	cout << "<BufferObject VertexBufferObject ID> = " << _vertexBufferObjectID << ";" << endl;
}