#pragma once
#include "Utility.h"
class PerlinNoise
{
private:
	PerlinNoise(void);
	~PerlinNoise(void);
	static void normalize2(GLfloat v[2]);
	static void normalize3(GLfloat v[3]);
public:
	static void init();
	static GLfloat noise1(GLfloat arg);
	static GLfloat noise2(GLfloat vec[2]);
	static GLfloat noise3(GLfloat vec[3]);
	static GLfloat PerlinNoise2D(GLfloat x,GLfloat y,GLfloat alpha,GLfloat beta,GLint n);
	static GLfloat PerlinNoise3D(GLfloat x,GLfloat y,GLfloat z,GLfloat alpha,GLfloat beta,GLint n);
};

