#pragma once

namespace YuiLib {

class ColliderData;

/// <summary>
/// 当たり判定データ:２Dのライン
/// </summary>
class ColliderDataLine2D : public ColliderData
{
public:
	ColliderDataLine2D(bool isTrigger);

	VECTOR startPoint;	// 始点
	VECTOR endPoint;	// 終点

protected:
	ColliderDataLine2D(ColliderData::Kind kind, bool isTrigger);
};

}