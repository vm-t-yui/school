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

	VECTOR start;	// 始点
	VECTOR end;		// 終点
};

}