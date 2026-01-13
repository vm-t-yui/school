#pragma once

struct Vector
{
	float x;
	float y;
};

Vector AddVector(Vector& v1, Vector& v2);
Vector SubVector(Vector& v1, Vector& v2);
Vector ScaleVector(Vector& vec, float size);
Vector NormalizeVector(Vector& vec);
float GetVectorLength(Vector& vec);
Vector NormalizeVector(Vector& vec);
