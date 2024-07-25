// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "YuiLib.h"

using namespace YuiLib;

/// <summary>
/// コンストラクタ
/// </summary>
ColliderDataOneWayLine2D::ColliderDataOneWayLine2D(bool isTrigger)
	: ColliderDataLine2D(ColliderData::Kind::OneWayLine2D, isTrigger)
	, isStartToEndUp(true)
{
	// 処理なし
}