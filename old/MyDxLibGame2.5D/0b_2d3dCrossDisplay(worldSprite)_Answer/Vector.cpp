#include <math.h>
#include "Vector.h"

//------------------------------//
// ベクトル同士の足し算.
//------------------------------//
Vector AddVector(Vector& v1, Vector& v2)
{
	Vector ret;
	ret.x = v1.x + v2.x;
	ret.y = v1.y + v2.y;
	return ret;
}

//------------------------------//
// ベクトル同士の引き算.
//------------------------------//
Vector SubVector(Vector& v1, Vector& v2)
{
	Vector ret;
	ret.x = v1.x - v2.x;
	ret.y = v1.y - v2.y;
	return ret;
}

//------------------------------//
// ベクトルのスカラー倍.
//------------------------------//
Vector ScaleVector(Vector& vec, float size)
{
	Vector ret;
	ret.x = vec.x * size;
	ret.y = vec.y * size;
	return ret;
}

//--------------------------------//
// ベクトルの長さ.
//--------------------------------//
float GetVectorLength(Vector& vec)
{
	return (float)sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

//------------------------------//
// ベクトルの正規化.
//------------------------------//
Vector NormalizeVector(Vector& vec)
{
	Vector ret;
	ret.x = ret.y = 0.0f;
	float length = GetVectorLength(vec);
	if (length != 0)
	{
		ret.x = vec.x / length;
		ret.y = vec.y / length;
	}
	return ret;
}
