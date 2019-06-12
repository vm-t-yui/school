//-----------------------------------------------------------------------------
// @brief  メイン処理(１ファイル形式).
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
/*
#include <stdio.h>

//-----------------------------------------------------------------------------
// @brief  ３次元ベクトル構造体.
//-----------------------------------------------------------------------------
struct Vector3
{
	float x;
	float y;
	float z;
};

//-----------------------------------------------------------------------------
// @brief  ３次元ベクトルv1とv2の加算結果を返す.
//-----------------------------------------------------------------------------
Vector3 Vector3Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 ret;
	ret.x = v1.x + v2.x;
	ret.y = v1.y + v2.y;
	ret.z = v1.z + v2.z;
	return ret;
}

//-----------------------------------------------------------------------------
// @brief  ３次元ベクトルv1とv2の減算結果を返す.
//-----------------------------------------------------------------------------
Vector3 Vector3Sub(const Vector3& v1, const Vector3& v2)
{
	Vector3 ret;
	ret.x = v1.x - v2.x;
	ret.y = v1.y - v2.y;
	ret.z = v1.z - v2.z;
	return ret;
}

//-----------------------------------------------------------------------------
// @brief  ３次元ベクトルvとfloat値scaleのスカラー積の結果を返す.
//-----------------------------------------------------------------------------
Vector3 Vector3Scale(const Vector3& v, float scale)
{
	Vector3 ret;
	ret.x = v.x * scale;
	ret.y = v.y * scale;
	ret.z = v.z * scale;
	return ret;
}

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
int simpleMain()
{
	Vector3 v1 = { 1, 2, 3 };
	Vector3 v2 = { 4, 5, 6 };
	float scale = 3.0f;
	printf("v1:(%f,%f,%f)\n", v1.x, v1.y, v1.z);
	printf("v2:(%f,%f,%f)\n", v2.x, v2.y, v2.z);
	printf("scale:%f\n", scale);
	puts("------");

	Vector3 Add = Vector3Add(v1, v2);
	Vector3 Sub = Vector3Sub(v1, v2);
	Vector3 Scale = Vector3Scale(v1, scale);

	printf("Vector3Add(v1, v2) = (%f, %f, %f) \n", Add.x, Add.y, Add.z);
	printf("Vector3Sub(v1, v2) = (%f, %f, %f) \n", Sub.x, Sub.y, Sub.z);
	printf("Vector3Scale(v1, scale) = (%f, %f, %f)\n", Scale.x, Scale.y, Scale.z);

	return 0;
}
*/