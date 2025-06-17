#pragma once

namespace YuiLib {

class ColliderData;

/// <summary>
/// 当たり判定データ:２Dのライン
/// </summary>
class ColliderDataLine2D : public ColliderData
{
public:
	ColliderDataLine2D();

	VECTOR startPoint;	// 始点
	VECTOR endPoint;	// 終点
};

}