// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "YuiLib.h"

using namespace YuiLib;

/// <summary>
/// コンストラクタ
/// </summary>
ColliderDataOneWayLine2D::ColliderDataOneWayLine2D()
	: ColliderDataLine2D(ColliderData::Kind::OneWayLine2D)
	, isStartToEndUp(true)
{
	// 処理なし
}