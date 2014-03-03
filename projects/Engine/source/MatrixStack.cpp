#include "MatrixStack.h"

MatrixStack* MatrixStack::instance = NULL;

MatrixStack::MatrixStack() {
}

MatrixStack::~MatrixStack() {

	_matrixStack.clear();
}

MatrixStack* MatrixStack::getInstance() {

	if(instance == NULL)
		instance = new MatrixStack();

	return instance;
}

void MatrixStack::destroyInstance() {

	delete instance;

	instance = NULL;
}

void MatrixStack::push(Matrix matrix) {

	_matrixStack.push_back(matrix);
}

Matrix MatrixStack::pop() {

	if(_matrixStack.empty() == true)
		return Matrix();

	Matrix matrix = _matrixStack.back();

	_matrixStack.pop_back();

	return matrix;
}

Matrix MatrixStack::pop(int count) {

	Matrix matrix;

	while(count-- > 0 && _matrixStack.empty() == false) {

		matrix = _matrixStack.back();

		 _matrixStack.pop_back();
	}

	return matrix;
}

Matrix MatrixStack::peek() {

	if(_matrixStack.empty() == true)
		return Matrix();

	return _matrixStack.back();
}

void MatrixStack::clean() {

	while(_matrixStack.empty() == false)
		_matrixStack.pop_back();
}

int MatrixStack::getMatrixCount() {

	return _matrixStack.size();
}

void MatrixStack::dump() {

	cout << "<MatrixStack Dump>" << endl;

	/* MatrixStack Size */
	cout << "<MatrixStack Size> = " << _matrixStack.size() << " ;" << endl;

	/* Matrix Stack */
	cout << "<MatrixStack Matrix Stack> = " << endl;
	for(list<Matrix>::iterator matrixIterator = _matrixStack.begin(); matrixIterator != _matrixStack.end(); matrixIterator++)
		matrixIterator->dump();
}