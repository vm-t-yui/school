//--------------------------------//
//	ベクトルライブラリ			  //
//						2008/1/9  //
//						由比　建　//
//--------------------------------//
//-----------------------------------------------------------------------------
// Copyright (c) 2016 Takeru Yui.
//-----------------------------------------------------------------------------

#ifndef __VECTOR_H__
#define __VECTOR_H__


//標準ライブラリ関数のインクルード
#include <stdio.h>
#include <math.h>


//三次元ベクトルクラス
class Vector3
{
	public:			
		float x;												//要素ｘ
		float y;												//要素ｙ
		float z;												//要素ｚ

		Vector3(){ x = 0.0f; y = 0.0f; z = 0.0f; }				//コンストラクタ
		Vector3(const Vector3& v){ x = v.x; y = v.y; z = v.z; }	//コピーコンストラクタ
		Vector3(float v);										//一つの引数で初期化
		Vector3(float aug_x,float aug_y,float aug_z);			//それぞれの引き数で初期化
		virtual ~Vector3(){}									//デストラクタ
			
		void SetX(float aug_x){ x = aug_x; }					//要素ｘをセット
		void SetY(float aug_y){ y = aug_y; }					//要素ｙをセット
		void SetZ(float aug_z){ z = aug_z; }					//要素ｚをセット
		void Set(float aug_x,float aug_y,float aug_z);			//要素をセット
		float Length();											//長さ
		Vector3 Normalize();									//正規化
		//Vector3 Proj(Vector3 &a);								//自分自身をAに投影する
		//Vector3 Perp(Vector3 &a);								//自分自身をAにPerpする
		
		void Print();											//値の表示

		//friendなので、アクセス演算子無視で直接アクセス可能
		Vector3 &operator = (const Vector3 &a);					//ベクトルの代入	
		friend bool operator == (Vector3 &a,Vector3 &b);		//ベクトルの比較
		friend Vector3 operator + (Vector3 &a,Vector3 &b);		//ベクトルの足し算	
		Vector3 &operator += (const Vector3 &a);				//足し算＋代入
		friend Vector3 operator - (Vector3 &a,Vector3 &b);		//ベクトルの引き算
		Vector3 &operator -= (const Vector3 &a);				//引き算＋代入
		friend float operator * (Vector3 &a,Vector3 &b);		//ベクトルの内積
		friend Vector3 operator * (Vector3 &a,float n);			//スカラー倍
		friend Vector3 operator * (float n,Vector3 &a);			//スカラー倍
		Vector3 &operator *= (const float n);					//スカラー倍＋代入
		friend Vector3 operator / (Vector3 &a,float n);			//スカラー除算
		friend Vector3 operator / (float n,Vector3 &a);			//スカラー除算
		Vector3 &operator /= (const float n);					//スカラー除算＋代入
		friend Vector3 operator % (Vector3& V1,Vector3& V2);	//ベクトルの外積	
		Vector3 &operator %= (const Vector3 &a);				//外籍＋代入
		//friend bool operator || (Vector3& a,Vector3& b);		//平行かどうか調べる
};

//四次元ベクトルクラス
//最低限の機能のみ実装。
class Vector4
{
	public:			
		float x;															//要素ｘ
		float y;															//要素ｙ
		float z;															//要素ｚ
		float w;															//要素w

		Vector4(){ x=0.0f;y=0.0f;z=0.0f;w=0.0f; }							//コンストラクタ
		Vector4(float a){ x=a;y=a;z=a;w=a; }
		Vector4(const Vector4& v){ x = v.x; y = v.y; z = v.z; w = v.w; }	//コピーコンストラクタ


		//それぞれの引数で初期化
		Vector4(float aug_x,float aug_y,float aug_z,float aug_w)
		{
			x = aug_x; y = aug_y; z = aug_z;w = aug_w;
		}
		virtual ~Vector4(){}												//デストラクタ

		//プリント
		void Print(){ printf("x:%f y:%f z:%f w:%f\n",x,y,z,w); }

		//引数セット
		void Set(float aug_x,float aug_y,float aug_z,float aug_w)
		{
			x = aug_x; y = aug_y; z = aug_z;w = aug_w;
		}
		//代入
		Vector4 &operator = (const Vector4 &a)
		{
			x = a.x; y = a.y; z = a.z;w = a.w;
			return *this;
		}

		friend Vector4 operator + (Vector4 &a,Vector4 &b);		//ベクトルの足し算	
		Vector4 &operator += (const Vector4 &a);				//足し算＋代入
		friend Vector4 operator - (Vector4 &a,Vector4 &b);		//ベクトルの引き算
		Vector4 &operator -= (const Vector4 &a);				//引き算＋代入
		friend float operator * (Vector4 &a,Vector4 &b);		//ベクトルの内積
		friend Vector4 operator * (Vector4 &a,float n);			//スカラー倍
		friend Vector4 operator * (float n,Vector4 &a);			//スカラー倍
		Vector4 &operator *= (const float n);					//スカラー倍＋代入
		friend Vector4 operator / (Vector4 &a,float n);			//スカラー除算
		friend Vector4 operator / (float n,Vector4 &a);			//スカラー除算
		Vector4 &operator /= (const float n);					//スカラー除算＋代入
};

//関数
float GetTwoVecRad(Vector3& a,Vector3& b);						//二つのベクトルの成す角を返す

#endif//__VECTOR_H__