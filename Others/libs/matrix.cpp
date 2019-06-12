//--------------------------------//
//	行列ライブラリ				  //
//						2008/1/9  //
//						由比　建　//
//--------------------------------//
//-----------------------------------------------------------------------------
// Copyright (c) 2016 Takeru Yui.
//-----------------------------------------------------------------------------

//ヘッダのインクルード
#include "matrix.h"

//----------------------------------------------------------------------------------------------//
//		33行列
//----------------------------------------------------------------------------------------------//
//--------------------------------//
//	コンストラクタ
//	引数	：初期化する値
//	役割	：引数一つで全て初期化する
//--------------------------------//
Matrix33::Matrix33(float n)
{
    for (int i = 0; i<9; i++)
        content.Array[i] = n;
}
//--------------------------------//
//コンストラクタ
//	引数	：初期化する値
//	役割	：三つの三次元ベクトルで初期化する
//--------------------------------//
Matrix33::Matrix33(Vector3 &v1, Vector3 &v2, Vector3 &v3)
{
    content.Array33[0][0] = v1.x;	content.Array33[1][0] = v2.x;
    content.Array33[0][1] = v1.y;	content.Array33[1][1] = v2.y;
    content.Array33[0][2] = v1.z;	content.Array33[1][2] = v2.z;

    content.Array33[2][0] = v3.x;
    content.Array33[2][1] = v3.y;
    content.Array33[2][2] = v3.z;
}


//--------------------------------//
//	関数名	：SetUnit
//	引数	：なし
//	戻り値	：なし
//	役割	：自分に単位行列をセットする
//--------------------------------//
void Matrix33::SetUnit()
{
    content.Array33[0][0] = 1;	content.Array33[0][1] = 0;
    content.Array33[1][0] = 0;	content.Array33[1][1] = 1;
    content.Array33[2][0] = 0;	content.Array33[2][1] = 0;

    content.Array33[0][2] = 0;
    content.Array33[1][2] = 0;
    content.Array33[2][2] = 1;
}

//--------------------------------//
//	関数名	：DetM
//	引数	：なし
//	戻り値	：なし
//	役割	：自分自身を行列式として計算した答えを返す
//--------------------------------//
float Matrix33::DetM()
{
    float ans;
    ans = (content.Array33[0][0] * content.Array33[1][1] * content.Array33[2][2])
        + (content.Array33[0][1] * content.Array33[1][2] * content.Array33[2][0])
        + (content.Array33[0][2] * content.Array33[1][0] * content.Array33[2][1])
        - (content.Array33[0][2] * content.Array33[1][1] * content.Array33[2][0])
        - (content.Array33[0][1] * content.Array33[1][0] * content.Array33[2][2])
        - (content.Array33[0][0] * content.Array33[1][2] * content.Array33[2][1]);
    return ans;
}


//--------------------------------//
//	関数名	：Print
//	引数	：なし
//	戻り値	：なし
//	役割	：値の表示
//--------------------------------//
void Matrix33::Print()
{
    for (int i = 0; i<3; i++)
    {
        printf("%d[", i);
        for (int j = 0; j<3; j++)
            printf("%f ", content.Array33[j][i]);
        printf("]\n");
    }
}

//----------------------------------------------------------------------------------------------//
//ここから演算子の	オーバーロード
//----------------------------------------------------------------------------------------------//

//--------------------------------//
//	オーバーロードした演算子：=
//	引数	：代入する４４行列
//	戻り値	：代入した自分自身
//	役割	：引数の４４行列を自分に代入する
//--------------------------------//
Matrix33 &Matrix33::operator = (const Matrix33 &a)
{
    for (int i = 0; i<3; i++)
    {
        for (int j = 0; j<3; j++)
            content.Array33[i][j] = a.content.Array33[i][j];
    }
    return *this;
}

//--------------------------------//
//	オーバーロードした演算子：*
//	引数	：掛け算する行列二つ
//	戻り値	：掛け算した後の行列
//	役割	：行列同士を掛け算して返す
//--------------------------------//
Matrix33 operator * (const Matrix33 &a, const Matrix33 &b)
{
    Matrix33 temp;
    for (int i = 0; i<3; i++)
    {
        for (int j = 0; j<3; j++)
        {
            temp.content.Array33[i][j] = (a.content.Array33[0][j] * b.content.Array33[i][0])
                + (a.content.Array33[1][j] * b.content.Array33[i][1])
                + (a.content.Array33[2][j] * b.content.Array33[i][2]);
        }
    }
    return temp;
}




//----------------------------------------------------------------------------------------------//
//		44行列
//----------------------------------------------------------------------------------------------//
//--------------------------------//
//コンストラクタ
//	引数	：初期化する値
//	役割	：引数一つで全て初期化する
//--------------------------------//
Matrix44::Matrix44(float n)
{
    for (int i = 0; i<16; i++)
        content.Array[i] = n;
}
//--------------------------------//
//コンストラクタ
//	引数	：初期化する値
//	役割	：四つの4次元ベクトルで初期化する（ゲーム用）
//--------------------------------//
Matrix44::Matrix44(Vector4 &v1, Vector4 &v2, Vector4 &v3, Vector4 &v4)
{
    content.Array44[0][0] = v1.x;	content.Array44[1][0] = v2.x;
    content.Array44[0][1] = v1.y;	content.Array44[1][1] = v2.y;
    content.Array44[0][2] = v1.z;	content.Array44[1][2] = v2.z;
    content.Array44[0][3] = v1.w;	content.Array44[1][3] = v2.w;

    content.Array44[2][0] = v3.x;	content.Array44[3][0] = v4.x;
    content.Array44[2][1] = v3.y;	content.Array44[3][1] = v4.y;
    content.Array44[2][2] = v3.z;	content.Array44[3][2] = v4.z;
    content.Array44[2][3] = v3.w;	content.Array44[3][3] = v4.w;
}
//--------------------------------//
//コンストラクタ
//	引数	：初期化する値
//役割四つの三次元ベクトルで初期化する（ゲーム用）
//--------------------------------//
Matrix44::Matrix44(Vector3 &v1, Vector3 &v2, Vector3 &v3, Vector3 &v4)
{
    content.Array44[0][0] = v1.x;	content.Array44[1][0] = v2.x;
    content.Array44[0][1] = v1.y;	content.Array44[1][1] = v2.y;
    content.Array44[0][2] = v1.z;	content.Array44[1][2] = v2.z;
    content.Array44[0][3] = 0;		content.Array44[1][3] = 0;

    content.Array44[2][0] = v3.x;	content.Array44[3][0] = v4.x;
    content.Array44[2][1] = v3.y;	content.Array44[3][1] = v4.y;
    content.Array44[2][2] = v3.z;	content.Array44[3][2] = v4.z;
    content.Array44[2][3] = 0;		content.Array44[3][3] = 1;
}


//--------------------------------//
//	関数名	：SetUnit
//	引数	：なし
//	戻り値	：なし
//	役割	：自分に単位行列をセットする
//--------------------------------//
void Matrix44::SetUnit()
{
    content.Array44[0][0] = 1;	content.Array44[0][1] = 0;
    content.Array44[1][0] = 0;	content.Array44[1][1] = 1;
    content.Array44[2][0] = 0;	content.Array44[2][1] = 0;
    content.Array44[3][0] = 0;	content.Array44[3][1] = 0;

    content.Array44[0][2] = 0;	content.Array44[0][3] = 0;
    content.Array44[1][2] = 0;	content.Array44[1][3] = 0;
    content.Array44[2][2] = 1;	content.Array44[2][3] = 0;
    content.Array44[3][2] = 0;	content.Array44[3][3] = 1;
}
//--------------------------------//
//	関数名	：Transpose
//	引数	：なし
//	戻り値	：転地したあとの行列
//	役割	：転地した自分自身を戻り値として返す
//--------------------------------//
Matrix44 Matrix44::Transpose()
{
    Matrix44 temp = *this;

    for (int i = 0; i<3; i++)
    {
        for (int j = 0; j<3; j++)
            temp.content.Array44[j][i] = content.Array44[3 - i][3 - j];
    }
    return temp;
}


//--------------------------------//
//	関数名	：Invert
//	引数	：なし
//	戻り値	：なし
//	役割	：自分自身に対する逆行列を返す(ゲーム専用)
//--------------------------------//
Matrix44 Matrix44::Invert()
{
    Matrix44 a(*this);
    Matrix44 b;

    b.SetUnit();

    unsigned int r, c;
    unsigned int cc;
    unsigned int rowMax;
    unsigned int row;
    float tmp;


    for (c = 0; c<4; c++)
    {

        rowMax = c;
        for (r = c + 1; r<4; r++)
        {
            if (fabs(a.content.Array44[c][r]) > fabs(a.content.Array44[c][rowMax]))
            {
                rowMax = r;
            }
        }

        if (a.content.Array44[rowMax][c] == 0.0f)
        {
            a.SetUnit();
            return a;
        }

        for (cc = 0; cc<4; cc++)
        {
            tmp = a.content.Array44[cc][c];
            a.content.Array44[cc][c] = a.content.Array44[cc][rowMax];
            a.content.Array44[cc][rowMax] = tmp;
            tmp = b.content.Array44[cc][c];
            b.content.Array44[cc][c] = b.content.Array44[cc][rowMax];
            b.content.Array44[cc][rowMax] = tmp;
        }

        tmp = a.content.Array44[c][c];
        for (cc = 0; cc<4; cc++)
        {
            a.content.Array44[cc][c] /= tmp;
            b.content.Array44[cc][c] /= tmp;
        }

        for (row = 0; row < 4; row++)
        {
            if (row != c)
            {
                tmp = a.content.Array44[c][row];
                for (cc = 0; cc<4; cc++)
                {
                    a.content.Array44[cc][row] -= a.content.Array44[cc][c] * tmp;
                    b.content.Array44[cc][row] -= b.content.Array44[cc][c] * tmp;
                }
            }
        }

    }

    return b;
}
//--------------------------------//
//	関数名	：DetM
//	引数	：なし
//	戻り値	：なし
//	役割	：自分自身を行列式として計算した答えを返す
//--------------------------------//
float Matrix44::DetM()
{
    Matrix44 mat = *this;
    float ans;
    Matrix33 det;

    for (int i = 0; i<3; i++)
    {
        for (int j = 0; j<3; j++)
            det.content.Array33[i][j] = mat.content.Array44[i + 1][j + 1];
    }

    ans = mat.content.Array44[0][0] * det.DetM()
        - mat.content.Array44[1][0] * det.DetM()
        + mat.content.Array44[2][0] * det.DetM()
        - mat.content.Array44[3][0] * det.DetM();

    return ans;
}

//--------------------------------//
//	関数名	：RotateRad
//	引数	：軸とする二点
//		回転するラジアン角
//	戻り値	：回転後の行列
//	役割	：引数の二点を通るベクトルを回転軸として、自身を引数角度分回転させた行列を返す
//--------------------------------//
void Matrix44::RotateRad(Vector3 &st, Vector3 &en, float rad)
{
    Vector3 axis;
    float x, y, z;
    float xSq, ySq, zSq;

    //ゼロ除算防止
    float sqrted = static_cast<float>(sqrt((en.x - st.x)*(en.x - st.x) + (en.y - st.y)*(en.y - st.y) + (en.z - st.z)*(en.z - st.z)));
    if (sqrted != 0)
    {
        axis.x = static_cast<float>((en.x - st.x) / sqrted);
        axis.y = static_cast<float>((en.y - st.y) / sqrted);
        axis.z = static_cast<float>((en.z - st.z) / sqrted);
    }
    else
    {
        axis.Set(0, 0, 0);
    }
    x = axis.x;		xSq = x * x;
    y = axis.y;		ySq = y * y;
    z = axis.z;		zSq = z * z;

    float a1 = xSq*(1.0f - cosf(rad)) + cosf(rad);
    float b1 = x*y*(1.0f - cosf(rad)) - z*sinf(rad);
    float c1 = x*z*(1.0f - cosf(rad)) + y*sinf(rad);

    float a2 = x*y*(1.0f - cosf(rad)) + z*sinf(rad);
    float b2 = ySq*(1.0f - cosf(rad)) + cosf(rad);
    float c2 = y*z*(1.0f - cosf(rad)) - x*sinf(rad);

    float a3 = x*z*(1.0f - cosf(rad)) - y*sinf(rad);
    float b3 = y*z*(1.0f - cosf(rad)) + x*sinf(rad);
    float c3 = zSq*(1 - cosf(rad)) + cosf(rad);

    content.Array44[0][0] = a1;	content.Array44[1][0] = b1;
    content.Array44[0][1] = a2;	content.Array44[1][1] = b2;
    content.Array44[0][2] = a3;	content.Array44[1][2] = b3;

    content.Array44[2][0] = c1;
    content.Array44[2][1] = c2;
    content.Array44[2][2] = c3;

    content.Array44[3][0] = (-1)*(a1 * st.x) - (b1 * st.y) - (c1 * st.z) + st.x;
    content.Array44[3][1] = (-1)*(a2 * st.x) - (b2 * st.y) - (c2 * st.z) + st.y;
    content.Array44[3][2] = (-1)*(a3 * st.x) - (b3 * st.y) - (c3 * st.z) + st.z;
}
//--------------------------------//
//	関数名	：RotateRad
//	引数	：軸とするベクトル
//		回転するラジアン角
//	戻り値	：回転後の行列
//	役割	：引数のベクトルを回転軸として、自身を引数角度分回転させた行列を返す
//--------------------------------//
void Matrix44::RotateRad(Vector3 &en, float rad)
{
    Matrix44 rotate;
    Vector3 axis;
    rotate.SetUnit();
    float x, y, z;
    float xSq, ySq, zSq;

    //ゼロ除算防止
    float sqrted = static_cast<float>(sqrt((en.x - 0)*(en.x - 0) + (en.y - 0)*(en.y - 0) + (en.z - 0)*(en.z - 0)));
    if (sqrted != 0)
    {
        axis.x = static_cast<float>((en.x - 0) / sqrted);
        axis.y = static_cast<float>((en.y - 0) / sqrted);
        axis.z = static_cast<float>((en.z - 0) / sqrted);
    }
    else
    {
        axis.Set(0, 0, 0);
    }

    x = axis.x;		xSq = x * x;
    y = axis.y;		ySq = y * y;
    z = axis.z;		zSq = z * z;

    float a1 = xSq*(1.0f - cosf(rad)) + cosf(rad);
    float b1 = x*y*(1.0f - cosf(rad)) - z*sinf(rad);
    float c1 = x*z*(1.0f - cosf(rad)) + y*sinf(rad);

    float a2 = x*y*(1.0f - cosf(rad)) + z*sinf(rad);
    float b2 = ySq*(1.0f - cosf(rad)) + cosf(rad);
    float c2 = y*z*(1.0f - cosf(rad)) - x*sinf(rad);

    float a3 = x*z*(1.0f - cosf(rad)) - y*sinf(rad);
    float b3 = y*z*(1.0f - cosf(rad)) + x*sinf(rad);
    float c3 = zSq*(1 - cosf(rad)) + cosf(rad);

    rotate.content.Array44[0][0] = a1;	rotate.content.Array44[1][0] = b1;
    rotate.content.Array44[0][1] = a2;	rotate.content.Array44[1][1] = b2;
    rotate.content.Array44[0][2] = a3;	rotate.content.Array44[1][2] = b3;

    rotate.content.Array44[2][0] = c1;
    rotate.content.Array44[2][1] = c2;
    rotate.content.Array44[2][2] = c3;

    rotate.content.Array44[3][0] = -(a1 * en.x) - (b1 * en.y) - (c1 * en.z) + en.x;
    rotate.content.Array44[3][1] = -(a2 * en.x) - (b2 * en.y) - (c2 * en.z) + en.y;
    rotate.content.Array44[3][2] = -(a3 * en.x) - (b3 * en.y) - (c3 * en.z) + en.z;

    *this = (*this) * rotate;
}
//--------------------------------//
//	関数名	：RotateRad XYZ
//	引数	：回転するラジアン角
//	戻り値	：回転後の行列
//	役割	：指定された軸（ｘｙｚ）を回転軸として、自身を引数角度分回転させる
//--------------------------------//
void Matrix44::RotateRadX(float rad)
{
    Matrix44 temp;
    temp.SetUnit();

    temp.content.Array44[1][1] = cosf(rad);
    temp.content.Array44[1][2] = sinf(rad);
    temp.content.Array44[2][1] = -sinf(rad);
    temp.content.Array44[2][2] = cosf(rad);

    Matrix44 mul;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            mul.content.Array44[i][j] = (content.Array44[0][j] * temp.content.Array44[i][0])
                + (content.Array44[1][j] * temp.content.Array44[i][1])
                + (content.Array44[2][j] * temp.content.Array44[i][2])
                + (content.Array44[3][j] * temp.content.Array44[i][3]);
        }
    }
    *this = mul;
}
void Matrix44::RotateRadY(float rad)
{
    Matrix44 temp;
    temp.SetUnit();

    temp.content.Array44[0][0] = cosf(rad);
    temp.content.Array44[0][2] = -sinf(rad);
    temp.content.Array44[2][0] = sinf(rad);
    temp.content.Array44[2][2] = cosf(rad);

    Matrix44 mul;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            mul.content.Array44[i][j] = (content.Array44[0][j] * temp.content.Array44[i][0])
                + (content.Array44[1][j] * temp.content.Array44[i][1])
                + (content.Array44[2][j] * temp.content.Array44[i][2])
                + (content.Array44[3][j] * temp.content.Array44[i][3]);
        }
    }
    *this = mul;
}
void Matrix44::RotateRadZ(float rad)
{
    Matrix44 temp;
    temp.SetUnit();

    temp.content.Array44[0][0] = cosf(rad);
    temp.content.Array44[0][1] = sinf(rad);
    temp.content.Array44[1][0] = -sinf(rad);
    temp.content.Array44[1][1] = cosf(rad);

    Matrix44 mul;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            mul.content.Array44[i][j] = (content.Array44[0][j] * temp.content.Array44[i][0])
                + (content.Array44[1][j] * temp.content.Array44[i][1])
                + (content.Array44[2][j] * temp.content.Array44[i][2])
                + (content.Array44[3][j] * temp.content.Array44[i][3]);
        }
    }
    *this = mul;
}


//--------------------------------//
//	関数名	：Translate
//	引数	：平行移動させるｘｙｚ
//	戻り値	：平行移動させた後の自分
//	役割	：自分を引数分平行移動させる
//--------------------------------//
void Matrix44::Translate(float x, float y, float z)
{
    Matrix44 temp;
    temp.SetUnit();

    temp.content.Array44[3][0] = x;
    temp.content.Array44[3][1] = y;
    temp.content.Array44[3][2] = z;
    temp.content.Array44[3][3] = 1.0f;

    Matrix44 mul;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            mul.content.Array44[i][j] = (content.Array44[0][j] * temp.content.Array44[i][0])
                + (content.Array44[1][j] * temp.content.Array44[i][1])
                + (content.Array44[2][j] * temp.content.Array44[i][2])
                + (content.Array44[3][j] * temp.content.Array44[i][3]);
        }
    }
    *this = mul;
}


//--------------------------------//
//	関数名	：ScaleChange
//	引数	：拡大縮小させるｘｙｚw
//	戻り値	：拡大縮小したあとの自分
//	役割	：自分を引数分拡大縮小する
//--------------------------------//
void Matrix44::ScaleChange(float x, float y, float z, float w)
{
    Matrix44 temp;
    temp.SetUnit();

    temp.content.Array44[0][0] = x;
    temp.content.Array44[1][1] = y;
    temp.content.Array44[2][2] = z;
    temp.content.Array44[3][3] = w;

    Matrix44 mul;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            mul.content.Array44[i][j] = (content.Array44[0][j] * temp.content.Array44[i][0])
                + (content.Array44[1][j] * temp.content.Array44[i][1])
                + (content.Array44[2][j] * temp.content.Array44[i][2])
                + (content.Array44[3][j] * temp.content.Array44[i][3]);
        }
    }
    *this = mul;
}



//--------------------------------//
//	関数名	：Set XYZW
//	引数	：セットする四次元ベクトル
//	戻り値	：なし
//	役割	：指定列に四次元ベクトルをセットする
//--------------------------------//
void Matrix44::SetX(Vector4& v)
{
    content.Array44[0][0] = v.x;	content.Array44[0][1] = v.y;
    content.Array44[0][2] = v.z;	content.Array44[0][3] = v.w;
}
void Matrix44::SetY(Vector4& v)
{
    content.Array44[1][0] = v.x;	content.Array44[1][1] = v.y;
    content.Array44[1][2] = v.z;	content.Array44[1][3] = v.w;
}
void Matrix44::SetZ(Vector4& v)
{
    content.Array44[2][0] = v.x;	content.Array44[2][1] = v.y;
    content.Array44[2][2] = v.z;	content.Array44[2][3] = v.w;
}
void Matrix44::SetW(Vector4& v)
{
    content.Array44[3][0] = v.x;	content.Array44[3][1] = v.y;
    content.Array44[3][2] = v.z;	content.Array44[3][3] = v.w;
}

//--------------------------------//
//	関数名	：Set XYZW
//	引数	：セットする三次元ベクトル
//	戻り値	：なし
//	役割	：指定列に三次元ベクトルをセットする
//--------------------------------//
void Matrix44::SetX(Vector3& v)
{
    content.Array44[0][0] = v.x;	content.Array44[0][1] = v.y;
    content.Array44[0][2] = v.z;	content.Array44[0][3] = 0;
}
void Matrix44::SetY(Vector3& v)
{
    content.Array44[1][0] = v.x;	content.Array44[1][1] = v.y;
    content.Array44[1][2] = v.z;	content.Array44[1][3] = 0;
}
void Matrix44::SetZ(Vector3& v)
{
    content.Array44[2][0] = v.x;	content.Array44[2][1] = v.y;
    content.Array44[2][2] = v.z;	content.Array44[2][3] = 0;
}
void Matrix44::SetW(Vector3& v)
{
    content.Array44[3][0] = v.x;	content.Array44[3][1] = v.y;
    content.Array44[3][2] = v.z;	content.Array44[3][3] = 1;
}


//--------------------------------//
//	関数名	：Print
//	引数	：なし
//	戻り値	：なし
//	役割	：値の表示
//--------------------------------//
void Matrix44::Print()
{
    for (int i = 0; i<4; i++)
    {
        printf("%d[", i);
        for (int j = 0; j<4; j++)
            printf("%f ", content.Array44[j][i]);
        printf("]\n");
    }
}



//----------------------------------------------------------------------------------------------//
//ここから演算子の	オーバーロード
//----------------------------------------------------------------------------------------------//





//--------------------------------//
//	オーバーロードした演算子：=
//	引数	：代入する４４行列
//	戻り値	：代入した自分自身
//	役割	：引数の４４行列を自分に代入する
//--------------------------------//
Matrix44 &Matrix44::operator = (const Matrix44 &a)
{
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
            content.Array44[i][j] = a.content.Array44[i][j];
    }
    return *this;
}


//--------------------------------//
//	オーバーロードした演算子：+
//	引数	：足し算する二つの４４行列
//	戻り値	：計算した後の４４行列
//	役割	：二つの４４行列を足した行列を返す
//--------------------------------//
Matrix44 operator + (const Matrix44 &a, const Matrix44 &b)
{
    Matrix44 temp;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
            temp.content.Array44[i][j] = a.content.Array44[i][j] + b.content.Array44[i][j];
    }
    return temp;
}

//--------------------------------//
//	オーバーロードした演算子：-
//	引数	：引き算する二つの４４行列
//	戻り値	：引いた後の４４行列
//	役割	：二つの４４行列を引き算する
//--------------------------------//
Matrix44 operator - (const Matrix44 &a, const Matrix44 &b)
{
    Matrix44 temp;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
            temp.content.Array44[i][j] = a.content.Array44[i][j] - b.content.Array44[i][j];
    }
    return temp;
}

//--------------------------------//
//	オーバーロードした演算子：*
//	引数	：実数倍するスカラーと４４行列
//	戻り値	：実数倍した４４行列
//	役割	：４４行列を実数倍する
//--------------------------------//
Matrix44 operator * (const Matrix44 &a, float n)
{
    Matrix44 temp;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
            temp.content.Array44[i][j] = a.content.Array44[i][j] * n;
    }
    return temp;

}
Matrix44 operator * (float n, const Matrix44 &a)
{
    Matrix44 temp;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
            temp.content.Array44[i][j] = a.content.Array44[i][j] * n;
    }
    return temp;
}

//--------------------------------//
//	オーバーロードした演算子：*
//	引数	：掛け算する行列二つ
//	戻り値	：掛け算した後の行列
//	役割	：行列同士を掛け算して返す
//--------------------------------//
Matrix44 operator * (const Matrix44 &a, const Matrix44 &b)
{
    Matrix44 temp;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            temp.content.Array44[i][j] = (a.content.Array44[0][j] * b.content.Array44[i][0])
                + (a.content.Array44[1][j] * b.content.Array44[i][1])
                + (a.content.Array44[2][j] * b.content.Array44[i][2])
                + (a.content.Array44[3][j] * b.content.Array44[i][3]);
        }
    }
    return temp;
}

//--------------------------------//
//	オーバーロードした演算子：*
//	引数	：掛け算する行列と四次元ベクトル
//	戻り値	：掛け算した後の四次元ベクトル
//	役割	：行列と四次元ベクトルを掛け算して返す
//--------------------------------//
Vector4 operator * (Matrix44 &m, Vector4 &v)
{
    Vector4 temp;

    temp.x = (m.content.Array44[0][0] * v.x) + (m.content.Array44[1][0] * v.y)
        + (m.content.Array44[2][0] * v.z) + (m.content.Array44[3][0] * v.w);

    temp.y = (m.content.Array44[0][1] * v.x) + (m.content.Array44[1][1] * v.y)
        + (m.content.Array44[2][1] * v.z) + (m.content.Array44[3][1] * v.w);

    temp.z = (m.content.Array44[0][2] * v.x) + (m.content.Array44[1][2] * v.y)
        + (m.content.Array44[2][2] * v.z) + (m.content.Array44[3][2] * v.w);

    temp.w = (m.content.Array44[0][3] * v.x) + (m.content.Array44[1][3] * v.y)
        + (m.content.Array44[2][3] * v.z) + (m.content.Array44[3][3] * v.w);

    return temp;
}
Vector4 operator * (Vector4 &v, Matrix44 &m)
{
    Vector4 temp;

    temp.x = (m.content.Array44[0][0] * v.x) + (m.content.Array44[1][0] * v.y)
        + (m.content.Array44[2][0] * v.z) + (m.content.Array44[3][0] * v.w);

    temp.y = (m.content.Array44[0][1] * v.x) + (m.content.Array44[1][1] * v.y)
        + (m.content.Array44[2][1] * v.z) + (m.content.Array44[3][1] * v.w);

    temp.z = (m.content.Array44[0][2] * v.x) + (m.content.Array44[1][2] * v.y)
        + (m.content.Array44[2][2] * v.z) + (m.content.Array44[3][2] * v.w);

    temp.w = (m.content.Array44[0][3] * v.x) + (m.content.Array44[1][3] * v.y)
        + (m.content.Array44[2][3] * v.z) + (m.content.Array44[3][3] * v.w);
    return temp;
}

//--------------------------------//
//	オーバーロードした演算子：*
//	引数	：掛け算する行列と三次元ベクトル
//	戻り値	：掛け算した後の三次元ベクトル
//	役割	：行列と三次元ベクトルを掛け算して返す(本来出来ないがｘｙｚ成分だけ無理やり返す)
//--------------------------------//
Vector3 operator * (Matrix44 &m, Vector3 &v)
{
    Vector4 temp;
    Vector4 v2(v.x, v.y, v.z, 1);

    temp = m * v2;

    return Vector3(temp.x, temp.y, temp.z);
}
Vector3 operator * (Vector3 &v, Matrix44 &m)
{
    Vector4 temp;
    Vector4 v2(v.x, v.y, v.z, 1);

    temp = m * v2;

    return Vector3(temp.x, temp.y, temp.z);
}


//------------------------------------------------------------------------------//
//	関数
//------------------------------------------------------------------------------//

//--------------------------------//
//	関数名	：RotateRadMatrix44
//	引数	：軸とする二点
//		回転するラジアン角
//	戻り値	：回転後の行列
//	役割	：引数の二点を通るベクトルを回転軸として、自身を引数角度分回転させる行列を返す
//--------------------------------//
Matrix44 RotateRadMatrix44(Vector3 &st, Vector3 &en, float rad)
{
    Matrix44 rotate;
    Vector3 axis;
    rotate.SetUnit();
    float x, y, z;
    float xSq, ySq, zSq;

    //ゼロ除算防止
    float sqrted = static_cast<float>(sqrt((en.x - st.x)*(en.x - st.x) + (en.y - st.y)*(en.y - st.y) + (en.z - st.z)*(en.z - st.z)));
    if (sqrted != 0)
    {
        axis.x = static_cast<float>((en.x - st.x) / sqrted);
        axis.y = static_cast<float>((en.y - st.y) / sqrted);
        axis.z = static_cast<float>((en.z - st.z) / sqrted);
    }
    else
    {
        axis.Set(0, 0, 0);
    }
    x = axis.x;		xSq = x * x;
    y = axis.y;		ySq = y * y;
    z = axis.z;		zSq = z * z;

    float a1 = xSq*(1.0f - cosf(rad)) + cosf(rad);
    float b1 = x*y*(1.0f - cosf(rad)) - z*sinf(rad);
    float c1 = x*z*(1.0f - cosf(rad)) + y*sinf(rad);

    float a2 = x*y*(1.0f - cosf(rad)) + z*sinf(rad);
    float b2 = ySq*(1.0f - cosf(rad)) + cosf(rad);
    float c2 = y*z*(1.0f - cosf(rad)) - x*sinf(rad);

    float a3 = x*z*(1.0f - cosf(rad)) - y*sinf(rad);
    float b3 = y*z*(1.0f - cosf(rad)) + x*sinf(rad);
    float c3 = zSq*(1 - cosf(rad)) + cosf(rad);

    rotate.content.Array44[0][0] = a1;	rotate.content.Array44[1][0] = b1;
    rotate.content.Array44[0][1] = a2;	rotate.content.Array44[1][1] = b2;
    rotate.content.Array44[0][2] = a3;	rotate.content.Array44[1][2] = b3;

    rotate.content.Array44[2][0] = c1;
    rotate.content.Array44[2][1] = c2;
    rotate.content.Array44[2][2] = c3;

    rotate.content.Array44[3][0] = (-1)*(a1 * st.x) - (b1 * st.y) - (c1 * st.z) + st.x;
    rotate.content.Array44[3][1] = (-1)*(a2 * st.x) - (b2 * st.y) - (c2 * st.z) + st.y;
    rotate.content.Array44[3][2] = (-1)*(a3 * st.x) - (b3 * st.y) - (c3 * st.z) + st.z;

    return rotate;
}
//--------------------------------//
//	関数名	：RotateRadMatrix44
//	引数	：軸とするベクトル
//		回転するラジアン角
//	戻り値	：回転後の行列
//	役割	：引数のベクトルを回転軸として、行列を引数角度分回転させる行列を返す
//--------------------------------//
Matrix44 RotateRadMatrix44(Vector3 &en, float rad)
{
    Matrix44 rotate;
    Vector3 axis;
    rotate.SetUnit();
    float x, y, z;
    float xSq, ySq, zSq;

    //ゼロ除算防止
    float sqrted = static_cast<float>(sqrt((en.x - 0)*(en.x - 0) + (en.y - 0)*(en.y - 0) + (en.z - 0)*(en.z - 0)));
    if (sqrted != 0)
    {
        axis.x = static_cast<float>((en.x - 0) / sqrted);
        axis.y = static_cast<float>((en.y - 0) / sqrted);
        axis.z = static_cast<float>((en.z - 0) / sqrted);
    }
    else
    {
        axis.Set(0, 0, 0);
    }
    x = axis.x;		xSq = x * x;
    y = axis.y;		ySq = y * y;
    z = axis.z;		zSq = z * z;

    float a1 = xSq*(1.0f - cosf(rad)) + cosf(rad);
    float b1 = x*y*(1.0f - cosf(rad)) - z*sinf(rad);
    float c1 = x*z*(1.0f - cosf(rad)) + y*sinf(rad);

    float a2 = x*y*(1.0f - cosf(rad)) + z*sinf(rad);
    float b2 = ySq*(1.0f - cosf(rad)) + cosf(rad);
    float c2 = y*z*(1.0f - cosf(rad)) - x*sinf(rad);

    float a3 = x*z*(1.0f - cosf(rad)) - y*sinf(rad);
    float b3 = y*z*(1.0f - cosf(rad)) + x*sinf(rad);
    float c3 = zSq*(1 - cosf(rad)) + cosf(rad);

    rotate.content.Array44[0][0] = a1;	rotate.content.Array44[1][0] = b1;
    rotate.content.Array44[0][1] = a2;	rotate.content.Array44[1][1] = b2;
    rotate.content.Array44[0][2] = a3;	rotate.content.Array44[1][2] = b3;

    rotate.content.Array44[2][0] = c1;
    rotate.content.Array44[2][1] = c2;
    rotate.content.Array44[2][2] = c3;

    rotate.content.Array44[3][0] = -(a1 * en.x) - (b1 * en.y) - (c1 * en.z) + en.x;
    rotate.content.Array44[3][1] = -(a2 * en.x) - (b2 * en.y) - (c2 * en.z) + en.y;
    rotate.content.Array44[3][2] = -(a3 * en.x) - (b3 * en.y) - (c3 * en.z) + en.z;

    return (rotate);
}

//--------------------------------//
//	関数名	：RotateRad XYZ
//	引数	：回転するラジアン角
//	戻り値	：回転後の行列
//	役割	：指定された軸（ｘｙｚ）を回転軸として、自身を引数角度分回転させた行列を返す
//--------------------------------//
Matrix44 RotateRadXMatrix44(float rad)
{
    Matrix44 temp;
    temp.SetUnit();

    temp.content.Array44[1][1] = cosf(rad);
    temp.content.Array44[1][2] = sinf(rad);
    temp.content.Array44[2][1] = -sinf(rad);
    temp.content.Array44[2][2] = cosf(rad);

    return temp;

}
Matrix44 RotateRadYMatrix44(float rad)
{
    Matrix44 temp;
    temp.SetUnit();

    temp.content.Array44[0][0] = cosf(rad);
    temp.content.Array44[0][2] = -sinf(rad);
    temp.content.Array44[2][0] = sinf(rad);
    temp.content.Array44[2][2] = cosf(rad);

    return temp;
}
Matrix44 RotateRadZMatrix44(float rad)
{
    Matrix44 temp;
    temp.SetUnit();

    temp.content.Array44[0][0] = cosf(rad);
    temp.content.Array44[0][1] = sinf(rad);
    temp.content.Array44[1][0] = -sinf(rad);
    temp.content.Array44[1][1] = cosf(rad);

    return temp;
}


//--------------------------------//
//	関数名	：TranslateMatrix44
//	引数	：平行移動させるｘｙｚ
//	戻り値	：平行移動させる行列
//	役割	：行列を引数分平行移動させる行列を返す
//--------------------------------//
Matrix44 TranslateMatrix44(float x, float y, float z)
{
    Matrix44 m;
    m.SetUnit();

    m.content.Array44[3][0] = x;
    m.content.Array44[3][1] = y;
    m.content.Array44[3][2] = z;

    return m;
}


//--------------------------------//
//	関数名	：ScaleChangeMatrix44
//	引数	：拡大縮小させるｘｙｚw
//	戻り値	：拡大縮小させる行列
//	役割	：行列を引数分拡大縮小させる行列を返す
//--------------------------------//
Matrix44 ScaleChangeMatrix44(float x, float y, float z, float w)
{
    Matrix44 m;
    m.SetUnit();

    m.content.Array44[0][0] = x;
    m.content.Array44[1][1] = y;
    m.content.Array44[2][2] = z;
    m.content.Array44[3][3] = w;

    return m;
}


//----------------------------------//
//	関数名	：LookAtMatrix44
//	引数	：本体・注目点・上方向成分
//	戻り値	：なし
//	役割	：あるベクトル位置をカメラとし、それからあるベクトルへビュー変換する行列を返す
//----------------------------------//
Matrix44 LookAtMatrix44(Vector3& pos, Vector3& look, Vector3& up)
{
    Matrix44 temp;
    temp.SetUnit();

    //-----------------------------------------------------//
    //---------GLの使用に合わせて無理やり転置する----------//
    //-----------------------------------------------------//
    //注視点からカメラへのベクトル（新たなｚ成分）
    Vector3 new_z = look - pos;
    new_z = new_z.Normalize();

    //注目点-カメラベクトル・仮の上方向ベクトルに対し、右方向に垂直なベクトル（新たなｘ成分GL仕様）
    Vector3 new_x = new_z % up.Normalize();
    new_x = new_x.Normalize();

    //上方向ベクトルの決定（新たなｙ成分）
    Vector3 new_y = new_x % new_z;					//上の二つが長さ１なので正規化の必要なし
    new_y = new_y.Normalize();

    //出来た成分をそれぞれ代入し、転置する。
    temp.content.Array44[0][0] = new_x.x;
    temp.content.Array44[1][0] = new_x.y;
    temp.content.Array44[2][0] = new_x.z;
    temp.content.Array44[3][0] = 0.0f;

    temp.content.Array44[0][1] = new_y.x;
    temp.content.Array44[1][1] = new_y.y;
    temp.content.Array44[2][1] = new_y.z;
    temp.content.Array44[3][1] = 0.0f;

    temp.content.Array44[0][2] = -new_z.x;
    temp.content.Array44[1][2] = -new_z.y;
    temp.content.Array44[2][2] = -new_z.z;
    temp.content.Array44[3][2] = 0.0f;

    temp.content.Array44[0][3] = 0;
    temp.content.Array44[1][3] = 0;
    temp.content.Array44[2][3] = 0;
    temp.content.Array44[3][3] = 1.0f;

    //-カメラポジション分平行移動させた行列をかける
    temp = temp * TranslateMatrix44(-pos.x, -pos.y, -pos.z);

    return temp;
}