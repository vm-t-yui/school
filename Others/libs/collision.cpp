//------------------------------------------//
//当たり判定用の関数が定義されているファイル
//------------------------------------------//
//-----------------------------------------------------------------------------
// Copyright (c) 2016 Takeru Yui.
//-----------------------------------------------------------------------------

#include "vector.h"
#include "collision.h"

#define EPSILON 0

//----------------------------------//
//	関数名	：PolygonNormal
//	引数	：三つの位置ベクトル
//	戻り値	：引数のベクトルで出来た三角形の法線
//	役割	：引数のベクトルで出来た三角形の法線を返す
//----------------------------------//
Vector3 PolygonNormal(Vector3 &v1, Vector3 &v2, Vector3 &v3)
{
    return Vector3((v1 - v2) % (v3 - v2));
}

//----------------------------------//
//	関数名	：CollisionSpherePolygon
//	引数	：球の位置　半径
//		　ポリゴンを形成する三つの点
//		　ポリゴンの法線
//	戻り値	：当たっている　true
//			当たっていない false
//	役割	：球とポリゴンの当たり判定
//----------------------------------//
bool CollisionSpherePolygon(Vector3& pos, float radius, Vector3& v1, Vector3& v2, Vector3& v3, Vector3& node)
{
    //最近接点との距離が半径より小さかったら当たっている
    Vector3 closs = ClosestPtPointPolygon(pos, v1, v2, v3);
    Vector3 clossto_sph = closs - pos;

    if (clossto_sph.Length() < radius)
    {
        node = closs;
        return true;
    }

    return false;
}

//----------------------------------//
//	関数名	：CollisionSpherePlane
//	引数	：球の位置　半径
//		　平面上の点一つ
//		　平面の法線
//	戻り値	：当たっている　HIT
//			平面の表にある SURFACE
//			平面の裏にある BACK
//	役割	：平面と球の当たり判定
//----------------------------------//
COLLISION CollisionSpherePlane(Vector3& pos, float radius, Vector3& v, Vector3& normal)
{
    float dot;									//内積用
    Vector3 plane_to_sphere = pos - v;			//平面-球ベクトル

                                                //平面-球ベクトルと面の法線の内積
    dot = plane_to_sphere * normal.Normalize();

    if ((dot - radius) > 0.0f)			//表側
    {
        return SURFACE;
    }
    else if ((dot + radius) < 0.0f)		//裏側
    {
        return BACK;
    }
    else								//重なってる
    {
        return HIT;
    }
}


//----------------------------------//
//	関数名	：CollisionSegmentPolygon
//	引数	：線分の始点　終点
//		　ポリゴンを形成する三つの点
//		　ポリゴンの法線
//	戻り値	：当たっている　true
//			当たっていない false
//	役割	：線分とポリゴンの当たり判定
//----------------------------------//
bool CollisionSegmentPolygon(Vector3& st, Vector3& en, Vector3& v1, Vector3& v2, Vector3& v3, Vector3& normal, Vector3& node)
{
    float a, b, t;
    Vector3 segment = en - st;			//線分の方向ベクトル
    segment = segment.Normalize();

    Vector3 segtopoly = v1 - st;		//線分からポリゴンに向かうベクトル

    a = segtopoly * normal;
    b = segment * normal;

    //平行なので交点なし
    if (b == 0.0f)
    {
        node.Set(0, 0, 0);
        return false;
    }
    //媒介変数ｔを求める
    t = a / b;

    //直線の方程式に代入し、ポリゴンのある平面との交点を求める
    Vector3 temp_node;
    temp_node.x = st.x + segment.x * t;
    temp_node.y = st.y + segment.y * t;
    temp_node.z = st.z + segment.z * t;

    //ポリゴンのある平面との交点がポリゴンの外なら交点無し
    if (!IsInPolygon(temp_node, v1, v2, v3, normal))
    {
        node.Set(0, 0, 0);
        return false;
    }

    //線分の中点から始点or終点の長さが
    //線分の中点から交点までの長さより大きいなら当たっていない
    Vector3 tempseg = en - st;		//元の大きさの線分ベクトル
    Vector3 half = st + tempseg.Normalize() * (tempseg.Length() / 2);
    Vector3 halftost = st - half;
    Vector3 halftonode = temp_node - half;

    if (halftost.Length() < halftonode.Length())
    {
        node.Set(0, 0, 0);
        return false;
    }

    //交点あり
    node = temp_node;			//交点をセットしてトゥルーを返す
    return true;

}

//----------------------------------//
//	関数名	：CollisionPolygonCapsuleColumn
//	引数	：連続球線分の始点　終点　半径
//		　ポリゴンの三点
//	戻り値	：当たっている　true
//			当たっていない false
//	役割	：カプセル型円柱（連続球）とポリゴンの当たり判定
//		　当たっていたら交点もセット
//----------------------------------//
bool CollisionPolygonCapsuleColumn(Vector3& st, Vector3& en, float col_radius, Vector3& v1, Vector3& v2, Vector3& v3, Vector3& normal, Vector3& node)
{
    //st-en線分とポリゴンが当たっているか調べる
    Vector3 sttoen = en - st;
    if (CollisionSegmentPolygon(st - sttoen.Normalize() * col_radius, en + sttoen.Normalize() * col_radius, v1, v2, v3, normal, node))
    {
        return true;
    }
    else
    {
        //st-en線分とポリゴンとの最近接点を調べる
        Vector3 closest_col;
        Vector3 closest_poly;
        ClosestPtSegmentPolygon(st, en, v1, v2, v3, closest_poly, closest_col);

        //最近接点同士の距離が半径よりも小さいとき
        //円柱とこれはぶつかっていることになる
        Vector3 closest_len = closest_poly - closest_col;

        if (closest_len.Length() < col_radius)
        {
            node = closest_poly;
            return true;
        }
        return false;
    }
}

//----------------------------------//
//	関数名	：CollisionSphereCapsuleColumn
//	引数	：連続球線分の始点　終点　半径
//		　調べたい球の位置　半径　交点取得用
//	戻り値	：当たっている　true
//			当たっていない false
//	役割	：カプセル型円柱（連続球）と球の当たり判定
//----------------------------------//
bool CollisionSphereCapsuleColumn(Vector3& st, Vector3& en, float col_radius, Vector3& pos, float sph_radius, Vector3& node)
{
    //st-en線分と球の中心との最近接点を調べる
    Vector3 closest_col;
    float t = 0;

    ClosestPtPointsSegment(pos, st, en, t, closest_col);

    //最近接点と球の中心との距離が、連続円半径＋球の半径より小さかった場合true
    Vector3 clossto_sph = pos - closest_col;
    if (clossto_sph.Length() < col_radius + sph_radius)
    {
        //交点セット
        Vector3 posto_closest = closest_col - pos;
        node = pos + posto_closest.Normalize() * (sph_radius);
        return true;
    }
    return false;
}

//----------------------------------//
//	関数名	：CollisionSphereColumn
//	引数	：円柱線分の始点　終点　半径
//		　調べたい球の位置　半径
//	戻り値	：当たっている　true
//			当たっていない false
//	役割	：円柱と球の当たり判定
//----------------------------------//
bool CollisionSphereColumn(Vector3& st, Vector3& en, Vector3& pos, float col_radius, float sph_radius, Vector3& node)
{
    Vector3 temp_node;
    //連続球に当たっている
    if (CollisionSphereCapsuleColumn(st, en, col_radius, pos, sph_radius, temp_node))
    {
        //かつ円柱始点平面-終点平面の間に近接点がある
        if ((CollisionSpherePlane(pos, sph_radius, st, en - st) == SURFACE ||
            CollisionSpherePlane(pos, sph_radius, st, en - st) == HIT)
            &&
            (CollisionSpherePlane(pos, sph_radius, en, st - en) == SURFACE ||
                CollisionSpherePlane(pos, sph_radius, en, st - en) == HIT))
        {
            //交点セット
            node = temp_node;
            return true;
        }
    }
    return false;
}

//----------------------------------//
//	関数名	：CollisionSphereFanColumn
//	引数	：円柱線分の始点　終点　半径
//			　調べたい球の位置　半径
//			　扇の始点、終点(クリップする平面の一点)
//	戻り値	：当たっている　true
//			当たっていない false
//	役割	：扇形円柱と球の当たり判定
//----------------------------------//
bool CollisionSphereFanColumn(Vector3& st, Vector3& en, Vector3& pos, float col_radius, float sph_radius, Vector3& node,
    Vector3& fanst, Vector3& fanen)
{
    //fanst fanenが同じだったら抜ける
    Vector3 fanlen = fanst - fanen;
    if (fanlen.Length() > -0.0001 && fanlen.Length() < 0.0001)
        return false;

    //@@問題点
    //fanst と fanenが円状でない場合どうすればいいのか

    Vector3 temp_node;
    //円柱に当たっている
    if (CollisionSphereColumn(st, en, pos, col_radius, sph_radius, temp_node))
    {
        float t = 0;		//ダミー
                            //円柱中心-扇の始点平面の法線を求める
                            //扇の始点-円柱中心線分の終点への最近接点　から　終点へ向かうベクトル
        Vector3 stnomal, clossest_colsegtost, clossest_fanst;
        ClosestPtPointsSegment(fanst, st, en, t, clossest_colsegtost);					//扇始点に対する円柱中心直線の最近接点
        ClosestPtPointsSegment(fanen, clossest_colsegtost, fanst, t, clossest_fanst);	//終点に対する上の最近接点-扇始点直線の最近接点
                                                                                        //上の最近接点-始点ベクトルの逆が法線
        stnomal = fanen - clossest_fanst;
        stnomal = stnomal.Normalize();

        //円柱中心-扇の終点平面の法線を求める
        //扇の終点-円柱中心線分と始点への最近接点　から　始点へ向かうベクトル
        Vector3 ennomal, clossest_colsegtoen, clossest_fanen;
        ClosestPtPointsSegment(fanen, st, en, t, clossest_colsegtoen);					//扇終点に対する円柱中心直線の最近接点
        ClosestPtPointsSegment(fanst, clossest_colsegtoen, fanen, t, clossest_fanen);	//始点に対する上の最近接点-扇終点直線の最近接点
                                                                                        //上の最近接点-終点ベクトルの逆が法線
        ennomal = fanst - clossest_fanen;
        ennomal = ennomal.Normalize();

        //円柱中心-扇の始点平面 円柱中心-扇の終点平面でクリッピング
        if ((CollisionSpherePlane(pos, sph_radius, fanst, stnomal) == SURFACE ||
            CollisionSpherePlane(pos, sph_radius, fanst, stnomal) == HIT)
            &&
            (CollisionSpherePlane(pos, sph_radius, fanen, ennomal) == SURFACE ||
                CollisionSpherePlane(pos, sph_radius, fanen, ennomal) == HIT))
        {
            //交点セット
            node = temp_node;

            return true;
        }
    }
    return false;
}


//----------------------------------//
//	関数名	：IsInPolygon
//	引数	：点の位置
//		　ポリゴンを形成する三つの点
//		　ポリゴンの法線
//	戻り値	：ポリゴン内　true
//			ポリゴン外 false
//	役割	：三角形の内外判定
//----------------------------------//
bool IsInPolygon(Vector3 P, Vector3& v1, Vector3& v2, Vector3& v3, Vector3& normal)
{
    float dot;							//外積用

                                        //平面の三点ABC(v1,v2,v3)と、球体と平面の接点Pで三角形を作り、
                                        //それぞれの三角形の法線が全て法線と同じ方向なら
                                        //Pはポリゴン内部にあることになる
    Vector3 temp_normal;

    //三角形ＰＡＢの法線ベクトルを作成
    temp_normal = PolygonNormal(P, v1, v2);
    dot = temp_normal * normal;

    //法線の向きとちがうなら外側
    if (dot < 0.0f)
        return false;

    //三角形ＰＢＣの法線ベクトルを作成
    temp_normal = PolygonNormal(P, v2, v3);
    dot = temp_normal * normal;

    //法線の向きとちがうなら外側
    if (dot < 0.0f)
        return false;

    //三角形ＰＣＡの法線ベクトルを作成
    temp_normal = PolygonNormal(P, v3, v1);
    dot = temp_normal * normal;

    //法線の向きとちがうなら外側
    if (dot < 0.0f)
        return false;

    //ココに来るということは
    //向きが全て同じなのでＯＫ
    return true;
}


//----------------------------------//
//	関数名	：PointPolyDistance
//	引数	：点の位置
//		　ポリゴンを形成する一点
//		　ポリゴンの法線
//	戻り値	：点とポリゴンの距離
//	役割	：点とポリゴンの距離を返す
//----------------------------------//
float PointPolyDistance(Vector3& point, Vector3& plane, Vector3& normal)
{
    float a;
    Vector3 vec;

    vec = point - plane;
    a = vec * normal;

    return static_cast<float>(fabs(a));
}


//----------------------------------//
//	関数名	：Clamp
//	引数	：ｸﾗﾝﾌﾟするフロート型の変数、最大　最小
//	戻り値	：ｸﾗﾝﾌﾟしたn
//	役割	：min maxの範囲でｎをｸﾗﾝﾌﾟ
//----------------------------------//
float Clamp(float n, float min, float max)
{
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

//----------------------------------//
//	関数名	：ClosestPtSegmentSegment
//	引数	：線分二つの始点終点
//		　sとtをセットする変数
//		　最近接点をセットするベクトル
//	戻り値	：関数(S1(s) = P1 + s*(Q1 - P1) と S2(t) = P2 + t*(Q2-P2))のsとt
//	役割	：二つの線分の最近接点をc1、c2にセット
//----------------------------------//
float ClosestPtSegmentSegment(Vector3& p1, Vector3& q1, Vector3& p2, Vector3&q2,
    float s, float t, Vector3& c1, Vector3& c2)
{
    Vector3 d1 = q1 - p1;		//線分S1の方向ベクトル
    Vector3 d2 = q2 - p2;		//線分S1の方向ベクトル
    Vector3 r = p1 - p2;
    float a = d1 * d1;			//線分S1の距離の平方、常に非負
    float e = d2 * d2;			//線分S2の距離の平方、常に非負
    float f = d2 * r;

    //片方あるいは両方の線分が点に縮退しているかどうかチェック
    if (a <= EPSILON && e <= EPSILON)
    {
        //両方の線分が点に縮退
        s = t = 0.0f;
        c1 = p1;
        c2 = p2;
        return ((c1 - c2) * (c1 - c2));
    }

    if (a <= EPSILON)
    {
        //最初の線分が点に縮退
        s = 0.0f;
        t = f / e;	 //s = 0 => t = (b*s + f) / e = f / e
        t = Clamp(t, 0.0f, 1.0f);
    }
    else
    {
        float c = d1 *r;
        if (e <= EPSILON)
        {
            //２番目の線分が点に縮退
            t = 0.0f;
            s = Clamp(-c / a, 0.0f, 1.0f);		// t = 0 => s = (b*t - c) / a = -c / a
        }
        else
        {
            //ここから一般的な縮退の場合を開始
            float b = d1 * d2;
            float tnom = b*s + f;
            if (tnom < 0.0f)
            {
                t = 0.0f;
                s = Clamp(-c / a, 0.0f, 1.0f);
            }
            else if (tnom > e)
            {
                t = 1.0f;
                s = Clamp((b - c) / a, 0.0f, 1.0f);
            }
            else
            {
                t = tnom / e;
            }
        }
    }
    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
    return (c1 - c2) * (c1 - c2);
}

//----------------------------------//
//	二つの直線の交点を返す
//	引数	：線分二つの始点終点
//	戻り値	：交点座標
//----------------------------------//
Vector3 GetTwoLineNode(Vector3& p1, Vector3& q1, Vector3& p2, Vector3& q2)
{
    //直線の方程式を求める
    //float a = y2 - y1;
    //float b = x1 - x2;
    //float c = -a * x1 - b * y1;
    float a1 = q1.y - p1.y;
    float b1 = p1.x - q1.x;
    float c1 = -a1 * p1.x - b1 * p1.y;
    float a2 = q2.y - p2.y;
    float b2 = p2.x - q2.x;
    float c2 = -a2 * p2.x - b2 * p2.y;

    return Vector3((c1 * b2 - c2 * b1) / (b1 * a2 - b2 * a1),
        (a1 * c2 - a2 * c1) / (b1 * a2 - b2 * a1), 0);
}

//----------------------------------//
//	関数名	：ClosestPtPointPolygon
//	引数	：近接点を求めたい点
//		　ポリゴンの三点
//	戻り値	：引数の点とポリゴンの最近接点
//	役割	：点とポリゴンの最近接点を返す
//----------------------------------//
Vector3 ClosestPtPointPolygon(Vector3& p, Vector3& a, Vector3& b, Vector3& c)
{
    //ボロノイ領域で各領域ごとにﾁｪｯｸ
    Vector3 ab = b - a;			//線分ab
    Vector3 ac = c - a;			//線分ac
    Vector3 ap = p - a;			//線分ap

                                //PがＡの外側の頂点領域の中にあるかチェック
    float d1 = ab * ap;
    float d2 = ac * ap;
    if (d1 <= 0.0f && d2 <= 0.0f) return a;

    //PがＢの外側の頂点領域の中にあるかどうかチェック
    Vector3 bp = p - b;
    float d3 = ab * bp;
    float d4 = ac * bp;
    if (d3 >= 0.0f && d4 <= d3) return b;

    //ＰがＡＢの辺領域の中にあるかどうかチェックし、あればPのＡＢ上に対する射影を返す
    float vc = d1*d4 - d3*d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
    {
        float v = d1 / (d1 - d3);
        return a + v * ab;		//重心座標(1-v,v,0)
    }

    //PがCの外側の頂点領域の中にあるかどうかチェック
    Vector3 cp = p - c;
    float d5 = ab * cp;
    float d6 = ac * cp;
    if (d6 >= 0.0f && d5 <= d6) return c;

    //PがACの辺領域の中にあるかどうかチェックし、あればPのAC上に対する射影を返す
    float vb = d5*d2 - d1*d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
    {
        float w = d2 / (d2 - d6);
        return a + w * ac;		//重心座標(1-w,0,w)
    }

    //PがBCの辺領域の中にあるかどうかチェックし、あればPのBC上に対する射影を返す
    float va = d3*d6 - d5*d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
    {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return b + w * (c - b);	//重心座標(0,1-w,w)
    }

    //ここに来るということはPは面領域の中にある。
    //Qをその重心座標(u,v,w)を用いて計算
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;

    return a + ab * v + ac * w;	// = u*a + v*b + w*c , u = va * denom = 1.0f - v - w
}


//----------------------------------//
//	関数名	：ClosestPtSegmentPolygon
//	引数	：線分の始点終点
//		　ポリゴンの三点
//	戻り値	：なし
//	役割	：線分とポリゴンの最近接点をpt_poly、pt_segにセット
//----------------------------------//
void ClosestPtSegmentPolygon(Vector3& st, Vector3& en, Vector3& a, Vector3& b, Vector3& c,
    Vector3& pt_poly, Vector3& pt_seg)
{
    //線分とポリゴンabcの各片との最近接点
    float s, t;	s = t = 0;			//ClosestPtSegmentSegmentに渡す一時変数
                                    //線分ab
    Vector3 c_segab;
    Vector3 c_abseg;
    ClosestPtSegmentSegment(a, b, st, en, s, t, c_abseg, c_segab);
    Vector3 segab = c_abseg - c_segab;

    //線分bc
    Vector3 c_segbc;
    Vector3 c_bcseg;
    ClosestPtSegmentSegment(b, c, st, en, s, t, c_bcseg, c_segbc);
    Vector3 segbc = c_bcseg - c_segbc;

    //線分ca
    Vector3 c_segca;
    Vector3 c_caseg;
    ClosestPtSegmentSegment(c, a, st, en, s, t, c_caseg, c_segca);
    Vector3 segca = c_caseg - c_segca;


    //ポリゴンと始点の最近接点
    Vector3 c_stpoly;
    c_stpoly = ClosestPtPointPolygon(st, a, b, c);
    Vector3 stpoly = st - c_stpoly;

    //ポリゴンと終点の最近接点
    Vector3 c_enpoly;
    c_enpoly = ClosestPtPointPolygon(en, a, b, c);
    Vector3 enpoly = en - c_stpoly;

    //これらを比べて一番近いペアがポリゴンと線分の最近接点
    Vector3* nearest;
    nearest = &segab;
    //一番近いペアを探す
    bool loop = true;
    while (loop)			//最近ペアの変更が行われる限りループ
    {
        loop = false;
        if (nearest->Length() > segab.Length())
        {
            nearest = &segab;
            loop = true;
        }
        else if (nearest->Length() > segbc.Length())
        {
            nearest = &segbc;
            loop = true;
        }
        else if (nearest->Length() > segca.Length())
        {
            nearest = &segca;
            loop = true;
        }
        else if (nearest->Length() > stpoly.Length())
        {
            nearest = &stpoly;
            loop = true;
        }
        else if (nearest->Length() > enpoly.Length())
        {
            nearest = &enpoly;
            loop = true;
        }
    }
    if (nearest == &segab)
    {
        pt_poly = c_abseg;
        pt_seg = c_segab;
    }
    else if (nearest == &segbc)
    {
        pt_poly = c_bcseg;
        pt_seg = c_segbc;
    }
    else if (nearest == &segca)
    {
        pt_poly = c_caseg;
        pt_seg = c_segca;
    }
    else if (nearest == &stpoly)
    {
        pt_poly = c_stpoly;
        pt_seg = st;
    }
    else if (nearest == &enpoly)
    {
        pt_poly = c_enpoly;
        pt_seg = en;
    }
    else
    {
        //ここに来るのは異常なのでゼロベクトルセット
        pt_poly.Set(0, 0, 0);
        pt_seg.Set(0, 0, 0);
    }
}

//----------------------------------//
//	関数名	：ClosestPtPointsSegment
//	引数	：線分の始点終点　求めたい点
//	戻り値	：なし
//	役割	：線分abおよび点ｃに対してab上の最近接点dを計算
//		　d(t) = a + t*(b - a)により表されるdの位置に対するパラメータtも返す
//----------------------------------//
void ClosestPtPointsSegment(Vector3& c, Vector3& a, Vector3& b, float &t, Vector3& d)
{
    Vector3 ab = b - a;
    //ab上にcを射影。しかしab * abによる除算は延期。最適化処理
    t = (c - a) * ab;
    if (t <= 0.0f)
    {
        //cは範囲(a,b)の外側、aの幅に射影され、aまでクランプ
        t = 0.0f;
        d = a;
    }
    else
    {
        float denom = ab * ab;		//denom = ||ab|| ^ 2なので常に非負
        if (t >= denom)
        {
            //cは範囲(a,b)の外側、bのそばに射影され、bまでクランプ
            t = 1.0f;
            d = b;
        }
        else
        {
            //cは範囲(a,b)の内側に射影され、延期した除算を実行
            t = t / denom;
            d = a + t * ab;
        }
    }
}


