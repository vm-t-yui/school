#pragma once

namespace YuiLib {

class ColliderData;

/// <summary>
/// 当たり判定データ:２Dのライン
/// </summary>
class ColliderDataOneWayLine2D : public ColliderDataLine2D
{
public:
	ColliderDataOneWayLine2D(bool isTrigger);

	bool	isStartToEndUp;	// 法線方向が始点→終点ラインの上方向か
};

}