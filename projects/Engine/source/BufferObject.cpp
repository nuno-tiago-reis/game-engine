#include "BufferObject.h"

BufferObject::BufferObject(const Vertex* vertices, GLint vertexCount) {

	_vertices = new Vertex[vertexCount];

	_vertexCount = vertexCount;

	memcpy(_vertices,vertices,sizeof(Vertex)*vertexCount);
}

BufferObject::~BufferObject() {

	delete[] _vertices;
}

void BufferObject::createBufferObject() {

	glGenVertexArrays(1, &_vertexArrayObjectID);
	glBindVertexArray(_vertexArrayObjectID);

	glGenBuffers(1, &_vertexBufferObjectID);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*_vertexCount, _vertices, GL_STATIC_DRAW);

	GLint offset = 0;
	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	offset += sizeof(_vertices[0].position);

	glEnableVertexAttribArray(NORMAL);
	glVertexAttribPointer(NORMAL, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offset);

	offset += sizeof(_vertices[0].normal);

	glEnableVertexAttribArray(TANGENT);
	glVertexAttribPointer(TANGENT, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offset);

	offset += sizeof(_vertices[0].tangent);

	glEnableVertexAttribArray(TEXTURE_UV);
	glVertexAttribPointer(TEXTURE_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offset);

	offset += sizeof(_vertices[0].textureUV);

	glEnableVertexAttribArray(AMBIENT);
	glVertexAttribPointer(AMBIENT, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offset);

	offset += sizeof(_vertices[0].ambient);

	glEnableVertexAttribArray(DIFFUSE);
	glVertexAttribPointer(DIFFUSE, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offset);

	offset += sizeof(_vertices[0].diffuse);

	glEnableVertexAttribArray(SPECULAR);
	glVertexAttribPointer(SPECULAR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offset);

	offset += sizeof(_vertices[0].specular);

	glEnableVertexAttribArray(SHININESS);
	glVertexAttribPointer(SHININESS, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offset);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDisableVertexAttribArray(POSITION);
	
	glDisableVertexAttribArray(NORMAL);
	glDisableVertexAttribArray(TANGENT);

	glDisableVertexAttribArray(TEXTURE_UV);

	glDisableVertexAttribArray(AMBIENT);
	glDisableVertexAttribArray(DIFFUSE);
	glDisableVertexAttribArray(SPECULAR);
	glDisableVertexAttribArray(SHININESS);

	Utility::checkOpenGLError("ERROR: Buffer Object creation failed.");
}

void BufferObject::destroyBufferObject() {

	glDisableVertexAttribArray(POSITION);
	
	glDisableVertexAttribArray(NORMAL);
	glDisableVertexAttribArray(TANGENT);

	glDisableVertexAttribArray(TEXTURE_UV);

	glDisableVertexAttribArray(AMBIENT);
	glDisableVertexAttribArray(DIFFUSE);
	glDisableVertexAttribArray(SPECULAR);
	glDisableVertexAttribArray(SHININESS);

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

		cout << "\tVertex " << i << " Position: ";
		cout << "\t[" << _vertices[i].position[0] << "][" << _vertices[i].position[1] << "][" << _vertices[i].position[2] << "][" << _vertices[i].position[3] << "]" << endl;
	}

	/* Buffer Object OpenGL IDs */
	cout << "<BufferObject VertexArrayObject ID> = " << _vertexArrayObjectID << " ;" << endl;
	cout << "<BufferObject VertexBufferObject ID> = " << _vertexBufferObjectID << ";" << endl;
}