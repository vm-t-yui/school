//-----------------------------------------------------------------------------
// @brief  ３次元ベクトル.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _VECTOR_H_
#define _VECTOR_H_

struct Vector3
{
	float x;
	float y;
	float z;
};

//-----------------------------------------------------------------------------
// プロトタイプ宣言.
//-----------------------------------------------------------------------------
Vector3 Vector3Add(const Vector3& v1, const Vector3& v2);
Vector3 Vector3Sub(const Vector3& v1, const Vector3& v2);
Vector3 Vector3Scale(const Vector3& v, float scale);

#endif // _VECTOR_H_