//--------------------------------//
//	行列ライブラリ				  //
//						2008/1/9  //
//						由比　建　//
//--------------------------------//
//-----------------------------------------------------------------------------
// Copyright (c) 2016 Takeru Yui.
//-----------------------------------------------------------------------------

#ifndef __MATRIX_H__
#define __MATRIX_H__

//標準ライブラリのインクルード
#include <stdio.h>
#include <memory.h>

#include "vector.h"

//二次元配列と一次元配列を同じ領域に確保する共用体(４４)
typedef union _Content44
{
	float Array[16];														//一次元配列
	float Array44[4][4];													//二次元配列
}Content44;

//二次元配列と一次元配列を同じ領域に確保する共用体(３３)
typedef union _Content33
{
	float Array[9];															//一次元配列
	float Array33[3][3];													//二次元配列
}Content33;

//33行列クラス(逆行列計算用に最低限の機能しか持たせない)
class Matrix33
{
	public:
		Content33 content;													//中身

		Matrix33(){}														//コンストラクタ
		Matrix33(const Matrix33& m){
			memcpy( &(this->content), &m.content, sizeof(Content33));}		//コピーコンストラクタ
		Matrix33(float n);													//コピーコンストラクタ
		Matrix33(Vector3 &v1, Vector3 &v2, Vector3 &v3);					//三つの三次元ベクトルで初期化
		virtual ~Matrix33(){}												//デストラクタ
		
		Matrix33 &operator = (const Matrix33 &a);							//行列の代入	
		friend Matrix33 operator * (const Matrix33 &a,const Matrix33 &b);	//行列同士の掛け算
		void SetUnit();														//単位行列のセット
		float DetM();														//行列式の計算
		void Print();														//値の表示

};

//44行列クラス
class Matrix44
{
	public:
		Content44 content;													//中身

		Matrix44(){}														//コンストラクタ
		Matrix44(const Matrix44& m){
			memcpy( &(this->content), &m.content, sizeof(Content44));}		//コピーコンストラクタ
		Matrix44(float n);													//引数一つで全部初期化
		Matrix44(Vector4 &v1, Vector4 &v2, Vector4 &v3,  Vector4 &v4);		//四つの四次元ベクトルで初期化
		Matrix44(Vector3 &v1, Vector3 &v2, Vector3 &v3,  Vector3 &v4);		//四つの三次元ベクトルで初期化（ゲーム用）
		virtual ~Matrix44(){}												//デストラクタ

		void SetUnit();														//単位行列のセット
		Matrix44 Transpose();												//転置行列
		Matrix44 Invert();													//逆行列
		float DetM();														//行列式の計算
		
		void RotateRad(Vector3 &st, Vector3 &en,float rad);					//ある二点を通るベクトルを回転軸とする回転行列（ラジアン）
		void RotateRad(Vector3 &en,float rad);								//あるベクトルを回転軸とする回転行列（ラジアン）
		void RotateRadX(float rad);											//X軸を回転軸とする回転行列（ラジアン）
		void RotateRadY(float rad);											//Y軸を回転軸とする回転行列（ラジアン）
		void RotateRadZ(float rad);											//Z軸を回転軸とする回転行列（ラジアン）
		void Translate(float x, float y, float z);							//平行移動
		void ScaleChange(float x, float y, float z, float w = 1.0f);		//拡大縮小
	
		void SetX(Vector4& v);												//一列目に値を四次元ベクトルの値をセットする
		void SetY(Vector4& v);												//二列目に値を四次元ベクトルの値をセットする
		void SetZ(Vector4& v);												//三列目に値を四次元ベクトルの値をセットする
		void SetW(Vector4& v);												//四列目に値を四次元ベクトルの値をセットする
		void SetX(Vector3& v);												//一列目に値を三次元ベクトルの値をセットする
		void SetY(Vector3& v);												//二列目に値を三次元ベクトルの値をセットする
		void SetZ(Vector3& v);												//三列目に値を三次元ベクトルの値をセットする
		void SetW(Vector3& v);												//四列目に値を三次元ベクトルの値をセットする
		void Print();														//値の表示

		//friendなので、アクセス演算子無視で直接アクセス可能
		Matrix44 &operator = (const Matrix44 &a);							//行列の代入	
		friend Matrix44 operator + (const Matrix44 &a,const Matrix44 &b);	//行列の足し算	
		friend Matrix44 operator - (const Matrix44 &a,const Matrix44 &b);	//行列の引き算
		friend Matrix44 operator * (const Matrix44 &a,float n);				//行列の実数倍
		friend Matrix44 operator * (float n,const Matrix44 &a);				//行列の実数倍
		friend Matrix44 operator * (const Matrix44 &a,const Matrix44 &b);	//行列同士の掛け算

		friend Vector4 operator * (Matrix44 &m,Vector4 &v);					//行列と四次元ベクトルの掛け算
		friend Vector4 operator * (Vector4 &v,Matrix44 &m);					//行列と四次元ベクトルの掛け算
		friend Vector3 operator * (Matrix44 &m,Vector3 &v);					//行列と三次元ベクトルの掛け算
		friend Vector3 operator * (Vector3 &v,Matrix44 &m);					//行列と三次元ベクトルの掛け算
};

//関数
Matrix44 RotateRadMatrix44(Vector3 &st, Vector3 &en,float rad);				//ある二点を通るベクトルを回転軸とする回転行列（ラジアン）
Matrix44 RotateRadMatrix44(Vector3 &en,float rad);							//あるベクトルを回転軸とする回転行列（ラジアン）
Matrix44 RotateRadXMatrix44(float rad);										//X軸を回転軸とする回転行列（ラジアン）
Matrix44 RotateRadYMatrix44(float rad);										//Y軸を回転軸とする回転行列（ラジアン）
Matrix44 RotateRadZMatrix44(float rad);										//Z軸を回転軸とする回転行列（ラジアン）
Matrix44 TranslateMatrix44(float x, float y, float z);						//平行移動
Matrix44 ScaleChangeMatrix44(float x, float y, float z, float w = 1.0f);	//拡大縮小
Matrix44 LookAtMatrix44(Vector3& pos ,Vector3& look,Vector3& up);			//擬似カメラからビュー変換行列を返す

#endif// __MATRIX_H__
