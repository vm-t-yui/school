#pragma once

namespace YuiLib {

class ColliderData;

/// <summary>
/// 当たり判定データ:２Dの円
/// </summary>
class ColliderDataCircle2D : public ColliderData
{
public:
	ColliderDataCircle2D();

	VECTOR		center;	// 円の中心点
	float		radius;	// 円の半径
};

}