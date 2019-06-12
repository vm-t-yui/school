//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "Vector.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
int main()
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
