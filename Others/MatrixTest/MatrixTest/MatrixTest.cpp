// MatrixTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#include <stdio.h>
#include <math.h>
#include <random>
#include <time.h>
#include "d3dx9math.h"
#include "matrix.h"

std::random_device rnd;     // 非決定的な乱数生成器

float GetRandomF()
{
	return (float)(rnd()) / 100000000.0f;
}

void PrintMyMatrix(const Matrix44& mat)
{
	printf("%f, %f, %f, %f\n", mat.content.Array[0], mat.content.Array[1], mat.content.Array[2], mat.content.Array[3]);
	printf("%f, %f, %f, %f\n", mat.content.Array[4], mat.content.Array[5], mat.content.Array[6], mat.content.Array[7]);
	printf("%f, %f, %f, %f\n", mat.content.Array[8], mat.content.Array[9], mat.content.Array[10], mat.content.Array[11]);
	printf("%f, %f, %f, %f\n", mat.content.Array[12], mat.content.Array[13], mat.content.Array[14], mat.content.Array[15]);
}
void PrintDirectXMatrix(const D3DXMATRIX& mat)
{
	printf("%f, %f, %f, %f\n", mat[0], mat[1], mat[2], mat[3]);
	printf("%f, %f, %f, %f\n", mat[4], mat[5], mat[6], mat[7]);
	printf("%f, %f, %f, %f\n", mat[8], mat[9], mat[10], mat[11]);
	printf("%f, %f, %f, %f\n", mat[12], mat[13], mat[14], mat[15]);
}

void SetDxMatToMyMat(const D3DXMATRIX dxmat, Matrix44& mat)
{
	Matrix44 newMat(
		Vector4(dxmat[0], dxmat[1], dxmat[2], dxmat[3]),
		Vector4(dxmat[4], dxmat[5], dxmat[6], dxmat[7]),
		Vector4(dxmat[8], dxmat[9], dxmat[10], dxmat[11]),
		Vector4(dxmat[12], dxmat[13], dxmat[14], dxmat[15])
		);
	mat = newMat;
}

int main(void)
{
	//srand((unsigned int)time(NULL));
	D3DXMATRIX dxMat1(
		GetRandomF(), GetRandomF(), GetRandomF(), GetRandomF(),
		GetRandomF(), GetRandomF(), GetRandomF(), GetRandomF(),
		GetRandomF(), GetRandomF(), GetRandomF(), GetRandomF(),
		GetRandomF(), GetRandomF(), GetRandomF(), GetRandomF()
		); 
	D3DXMATRIX dxMat2(
		GetRandomF(), GetRandomF(), GetRandomF(), GetRandomF(),
		GetRandomF(), GetRandomF(), GetRandomF(), GetRandomF(),
		GetRandomF(), GetRandomF(), GetRandomF(), GetRandomF(),
		GetRandomF(), GetRandomF(), GetRandomF(), GetRandomF()
		);
	Matrix44 mat1;
	Matrix44 mat2;
	SetDxMatToMyMat(dxMat1, mat1);
	SetDxMatToMyMat(dxMat2, mat2);

	puts("[mat1]");
	mat1.Print();
	puts("[mat2]");
	mat2.Print();
	puts("=============");
	puts("[mat1 + mat2](origin)");
	Matrix44 ans = mat1 + mat2;
	PrintMyMatrix(ans);
	puts("[mat1 + mat2](DirectX)");
	D3DXMATRIX ansDx = dxMat1 + dxMat2;
	PrintDirectXMatrix(ansDx);
	puts("=============");
	puts("[mat1 - mat2](origin)");
	ans = mat1 - mat2;
	PrintMyMatrix(ans);
	puts("[mat1 - mat2](DirectX)");
	ansDx = dxMat1 - dxMat2;
	PrintDirectXMatrix(ansDx);
	puts("=============");
	puts("[mat1 * mat2](origin)");
	ans = mat1 * mat2;
	PrintMyMatrix(ans);
	puts("[mat1 * mat2](DirectX)");
	D3DXMatrixMultiply(&ansDx, &dxMat1, &dxMat2);
	PrintDirectXMatrix(ansDx);

	puts("=============");
	puts("[mat1 identify](origin)");
	ans = mat1;
	ans.SetUnit();
	PrintMyMatrix(ans);
	puts("[mat1 identify](DirectX)");
	D3DXMatrixIdentity(&ansDx);
	PrintDirectXMatrix(ansDx);

	puts("=============");
	puts("[mat1 invert](origin)");
	ans = mat1;
	ans = ans.Invert();
	PrintMyMatrix(ans);
	puts("[mat1 invert](DirectX)");
	ansDx = dxMat1;
	D3DXMatrixInverse(&ansDx, NULL, &ansDx);
	PrintDirectXMatrix(ansDx);

	float x = GetRandomF();
	float y = GetRandomF();
	float z = GetRandomF();
	D3DXMATRIX tmp;
	puts("=============");
	puts("[mat1 Translate](origin)");
	ans = mat1;
	ans.Translate(x,y,z);
	PrintMyMatrix(ans);
	puts("[mat1 Translate](DirectX)");
	D3DXMatrixTranslation(&tmp, x, y, z);
	ansDx = dxMat1 * tmp;
	PrintDirectXMatrix(ansDx);

	puts("=============");
	puts("[mat1 RotateX](origin)");
	ans = mat1;
	ans.RotateRadX(x);
	PrintMyMatrix(ans);
	puts("[mat1 RotateX](DirectX)");
	D3DXMatrixRotationX(&tmp, x);
	ansDx = dxMat1 * tmp;
	PrintDirectXMatrix(ansDx);

	puts("=============");
	puts("[mat1 RotateY](origin)");
	ans = mat1;
	ans.RotateRadY(y);
	PrintMyMatrix(ans);
	puts("[mat1 RotateY](DirectX)");
	D3DXMatrixRotationY(&tmp, y);
	ansDx = dxMat1 * tmp;
	PrintDirectXMatrix(ansDx);

	puts("=============");
	puts("[mat1 RotateZ](origin)");
	ans = mat1;
	ans.RotateRadZ(z);
	PrintMyMatrix(ans);
	puts("[mat1 RotateZ](DirectX)");
	D3DXMatrixRotationZ(&tmp, z);
	ansDx = dxMat1 * tmp;
	PrintDirectXMatrix(ansDx);


	puts("=============");
	Vector3 vec(x, y, z);
	D3DXVECTOR3 vecDx(x, y, z);
	float angle = GetRandomF();
	puts("[mat1 Rotate](origin)");
	ans = mat1;
	ans.RotateRad(vec, angle);
	PrintMyMatrix(ans);
	puts("[mat1 Rotate](DirectX)");
	D3DXMatrixRotationAxis(&tmp, &vecDx, angle);
	ansDx = dxMat1 * tmp;
	PrintDirectXMatrix(ansDx);

	puts("=============");
	puts("[mat1 Scale](origin)");
	ans = mat1;
	ans.ScaleChange(x,y,z);
	PrintMyMatrix(ans);
	puts("[mat1 Scale](DirectX)");
	D3DXMatrixScaling(&tmp, x,y,z);
	ansDx = dxMat1 * tmp;
	PrintDirectXMatrix(ansDx);

	puts("=============");
	D3DXVECTOR4 vecDx4(x, y, z, 0);
	puts("[mat1 multi vec](origin)");
	vec = mat1 * vec;
	printf("%f, %f, %f\n", vec.x, vec.y, vec.z);
	puts("[mat1 multi vec](DirectX)");
	ansDx = dxMat1;
	D3DXVec3Transform(&vecDx4, &vecDx, &ansDx);
	printf("%f, %f, %f\n", vecDx4.x, vecDx4.y, vecDx4.z);

	return 0;
}

