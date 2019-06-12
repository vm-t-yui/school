//------------------------------------------------------//
//	当たり判定の関数の定数とプロトタイプ宣言
//------------------------------------------------------//
//-----------------------------------------------------------------------------
// Copyright (c) 2016 Takeru Yui.
//-----------------------------------------------------------------------------

#ifndef __COLLISION_H__
#define __COLLISION_H__


//当たり判定用定数
enum COLLISION		//どの場所にいるか
{
    SURFACE,													//表
    BACK,														//裏
    HIT,														//当たっている
    COLLISION_MAX												//番兵
};
enum COLLISION_TYPE	//当たり判定の種類
{
    COL_TYPE_NONE = 0,											//当たり判定なし
    COL_TYPE_POLYGON,											//ポリゴン判定
    COL_TYPE_SPHERE,											//球判定
    COL_TYPE_MAX												//番兵
};

//	球とポリゴン
bool CollisionSpherePolygon(Vector3& pos, float radius, Vector3& v1, Vector3& v2, Vector3& v3, Vector3& node);
//	球と平面
COLLISION CollisionSpherePlane(Vector3& pos, float radius, Vector3& v, Vector3& normal);
//	線分とポリゴン
bool CollisionSegmentPolygon(Vector3& st, Vector3& en, Vector3& v1, Vector3& v2, Vector3& v3, Vector3& normal, Vector3& node);
//	三角形の内外判定
bool IsInPolygon(Vector3 P, Vector3& v1, Vector3& v2, Vector3& v3, Vector3& normal);
//	円柱（連続球）とポリゴン
bool CollisionPolygonCapsuleColumn(Vector3& st, Vector3& en, float col_radius, Vector3& v1, Vector3& v2, Vector3& v3, Vector3& normal, Vector3& node);
//	円柱（連続球）と球
bool CollisionSphereCapsuleColumn(Vector3& st, Vector3& en, float col_radius, Vector3& pos, float sph_radius, Vector3& node = Vector3(0, 0, 0));
//	円柱と球
bool CollisionSphereColumn(Vector3& st, Vector3& en, Vector3& pos, float col_radius, float sph_radius, Vector3& node);
//扇形円柱と球
bool CollisionSphereFanColumn(Vector3& st, Vector3& en, Vector3& pos, float col_radius, float sph_radius, Vector3& node, Vector3& fanst, Vector3& fanen);
//	点とポリゴンの距離
float PointPolyDistance(Vector3& point, Vector3& plane, Vector3& normal);
//	二つの直線の交点
Vector3 GetTwoLineNode(Vector3& p1, Vector3& q1, Vector3& p2, Vector3&q2);

//-----------------------------------------------------------------------------------------------------------
//	最近接点計算
//-----------------------------------------------------------------------------------------------------------

//min maxの範囲でクランプ
float Clamp(float n, float min, float max);
//二つの線分の最近接点
float ClosestPtSegmentSegment(Vector3& p1, Vector3& q1, Vector3& p2, Vector3&q2, float s, float t, Vector3& c1, Vector3& c2);
//点とポリゴンの最近接点
Vector3 ClosestPtPointPolygon(Vector3& pt, Vector3& a, Vector3& b, Vector3& c);
//線分とポリゴンの最近接点
void ClosestPtSegmentPolygon(Vector3& st, Vector3& en, Vector3& a, Vector3& b, Vector3& c, Vector3& pt_poly, Vector3& pt_seg);
//線分と点の最近接点
void ClosestPtPointsSegment(Vector3& c, Vector3& a, Vector3& b, float &t, Vector3& d);



#endif		//__COLLISION_H__
